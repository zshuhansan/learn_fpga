module icache (
    input  wire        clk,
    input  wire        resetn,

    // 处理器看到的是一个简化后的请求/响应接口。
    input  wire        cpu_req_valid,
    input  wire        cpu_req_en,
    input  wire [31:0] cpu_req_addr,
    output wire        cpu_req_ready,
    output wire [31:0] cpu_resp_data,
    output wire        cpu_resp_valid,

    // 外部存储侧使用 AXI4 只读主机接口。
    output wire [31:0] axi_araddr,
    output wire [7:0]  axi_arlen,
    output wire [2:0]  axi_arsize,
    output wire [1:0]  axi_arburst,
    output wire        axi_arvalid,
    input  wire        axi_arready,
    input  wire [31:0] axi_rdata,
    input  wire [1:0]  axi_rresp,
    input  wire        axi_rlast,
    input  wire        axi_rvalid,
    output wire        axi_rready
);

    wire icache_unused_ok = &{1'b0, cpu_req_addr[1:0], axi_rresp};

    // 这是一颗 8KB、32B cache line、2 路组相联的 ICache。
    // 地址划分方式为：Tag[31:12]、Index[11:5]、Offset[4:0]。
    
    reg [255:0] data_array_way0 [0:127];
    reg [19:0]  tag_array_way0  [0:127];
    reg         valid_array_way0 [0:127];

    reg [255:0] data_array_way1 [0:127];
    reg [19:0]  tag_array_way1  [0:127];
    reg         valid_array_way1 [0:127];

    // lru_array 记录下一次 miss 应优先替换哪一路。
    reg         lru_array [0:127];

    wire [19:0] req_tag = cpu_req_addr[31:12];
    wire [6:0]  req_index = cpu_req_addr[11:5];
    wire [2:0]  req_word_offset = cpu_req_addr[4:2];
    wire        req_fire = cpu_req_valid && cpu_req_en;

    wire [255:0] cache_line_way0  = data_array_way0[req_index];
    wire [19:0]  cache_tag_way0   = tag_array_way0[req_index];
    wire         cache_valid_way0 = valid_array_way0[req_index];

    wire [255:0] cache_line_way1  = data_array_way1[req_index];
    wire [19:0]  cache_tag_way1   = tag_array_way1[req_index];
    wire         cache_valid_way1 = valid_array_way1[req_index];

    // 命中判断只在当前索引对应的两个 way 中比较 tag。
    wire hit_way0 = cache_valid_way0 && (cache_tag_way0 == req_tag);
    wire hit_way1_raw = cache_valid_way1 && (cache_tag_way1 == req_tag);
`ifdef CONFIG_ICACHE_ONEWAY
    wire hit_way1 = 1'b0;
`else
    wire hit_way1 = hit_way1_raw;
`endif
    
    wire cache_hit = hit_way0 || hit_way1;
    // 把状态和本模块的 localparam/reg 声明提前，避免先用后定义导致综合报错。
    localparam IDLE       = 3'd0;
    localparam AR_REQ     = 3'd1;
    localparam R_WAIT     = 3'd2;
    localparam REFILL_END = 3'd3;

    reg [2:0] state;
    reg [2:0] burst_cnt;
    reg [255:0] refill_line;
    reg [31:0] miss_addr;
    reg        replace_way;
    reg [2:0]  miss_word_offset;
    reg [31:0] resp_data_r;
    reg        resp_valid_r;

    wire is_hit  = (state == IDLE) && req_fire && cache_hit;
    wire is_miss = (state == IDLE) && req_fire && !cache_hit;

    wire [255:0] cache_line = hit_way0 ? cache_line_way0 : cache_line_way1;

    // miss 处理流程分成发 AR、等 R、完成回填三个阶段。

`ifdef BENCH
    integer hit_count;
    integer miss_count;
`endif

    always @(posedge clk) begin
        if (resetn && is_hit) begin
            // 命中的 way 变成最近使用，另一条就成为下次优先替换对象。
            if (hit_way0) lru_array[req_index] <= 1'b1;
            if (hit_way1) lru_array[req_index] <= 1'b0;
        end
    end

    integer i;
`ifdef CONFIG_INITIALIZE
    initial begin
        // 仿真初始化时把 valid 清零，避免未定义内容被误判为命中。
        for (i = 0; i < 128; i = i + 1) begin
            valid_array_way0[i] = 1'b0;
            valid_array_way1[i] = 1'b0;
            lru_array[i]        = 1'b0;
        end
    end
`endif

    always @(posedge clk) begin
        if (!resetn) begin
            state <= IDLE;
            burst_cnt <= 3'd0;
            refill_line <= 256'd0;
            miss_addr <= 32'd0;
            replace_way <= 1'b0;
            miss_word_offset <= 3'd0;
            resp_data_r <= 32'd0;
            resp_valid_r <= 1'b0;
`ifdef BENCH
            hit_count <= 0;
            miss_count <= 0;
`endif
        end else begin
            resp_valid_r <= 1'b0;
`ifdef BENCH
            if (state == IDLE && req_fire) begin
                if (is_hit) begin
                    hit_count <= hit_count + 1;
                end else if (is_miss) begin
                    miss_count <= miss_count + 1;
                end
            end
`endif
            case (state)
                IDLE: begin
                    if (is_hit) begin
                        // hit 时直接从命中的 cache line 里抽取对应字返回给 CPU。
                        resp_valid_r <= 1'b1;
                        case (req_word_offset)
                            3'd0: resp_data_r <= cache_line[31:0];
                            3'd1: resp_data_r <= cache_line[63:32];
                            3'd2: resp_data_r <= cache_line[95:64];
                            3'd3: resp_data_r <= cache_line[127:96];
                            3'd4: resp_data_r <= cache_line[159:128];
                            3'd5: resp_data_r <= cache_line[191:160];
                            3'd6: resp_data_r <= cache_line[223:192];
                            default: resp_data_r <= cache_line[255:224];
                        endcase
                    end
                    if (is_miss) begin
`ifdef CONFIG_EVTLOG
                        $display("ICACHE_MISS addr=%h tag=%h idx=%0d repl_way=%0d", {req_tag, req_index, 5'b0}, req_tag, req_index, lru_array[req_index]);
`endif
                        // miss 地址会按整条 cache line 对齐，后面 AXI 回填整条 32B 数据。
                        state <= AR_REQ;
                        miss_addr <= {req_tag, req_index, 5'b00000};
                        miss_word_offset <= req_word_offset;
                        burst_cnt <= 3'd0;
                        if (!valid_array_way0[req_index])
                            replace_way <= 1'b0;
                        else if (!valid_array_way1[req_index])
                            replace_way <= 1'b1;
                        else
                            replace_way <= lru_array[req_index];
                    end
                end

                AR_REQ: begin
                    if (axi_arready && axi_arvalid) begin
                        // 地址握手完成后进入读数据等待阶段。
                        state <= R_WAIT;
                    end
                end

                R_WAIT: begin
                    if (axi_rvalid && axi_rready) begin
                        // AXI 每拍返回一个 32 位 beat，按 burst_cnt 写入回填缓冲。
                        case (burst_cnt)
                            3'd0: refill_line[31:0]   <= axi_rdata;
                            3'd1: refill_line[63:32]  <= axi_rdata;
                            3'd2: refill_line[95:64]  <= axi_rdata;
                            3'd3: refill_line[127:96] <= axi_rdata;
                            3'd4: refill_line[159:128]<= axi_rdata;
                            3'd5: refill_line[191:160]<= axi_rdata;
                            3'd6: refill_line[223:192]<= axi_rdata;
                            3'd7: refill_line[255:224]<= axi_rdata;
                        endcase
                        
                        burst_cnt <= burst_cnt + 3'd1;

                        if (axi_rlast) begin
                            // 收到最后一个 beat 后，下一拍把整条 line 正式写回数组。
                            state <= REFILL_END;
                        end
                    end
                end

                REFILL_END: begin
`ifdef CONFIG_ICACHE_ONEWAY
                    valid_array_way0[miss_addr[11:5]] <= 1'b1;
                    tag_array_way0[miss_addr[11:5]]   <= miss_addr[31:12];
                    data_array_way0[miss_addr[11:5]]  <= refill_line;
                    lru_array[miss_addr[11:5]]        <= 1'b1;
`else
                    if (replace_way == 1'b0) begin
                        valid_array_way0[miss_addr[11:5]] <= 1'b1;
                        tag_array_way0[miss_addr[11:5]]   <= miss_addr[31:12];
                        data_array_way0[miss_addr[11:5]]  <= refill_line;
                        lru_array[miss_addr[11:5]]        <= 1'b1;
                    end else begin
                        valid_array_way1[miss_addr[11:5]] <= 1'b1;
                        tag_array_way1[miss_addr[11:5]]   <= miss_addr[31:12];
                        data_array_way1[miss_addr[11:5]]  <= refill_line;
                        lru_array[miss_addr[11:5]]        <= 1'b0;
                    end
`endif
                    // 回填完成的同一拍就把请求字回送给 CPU，减少额外等待。
                    resp_valid_r <= 1'b1;
                    case (miss_word_offset)
                        3'd0: resp_data_r <= refill_line[31:0];
                        3'd1: resp_data_r <= refill_line[63:32];
                        3'd2: resp_data_r <= refill_line[95:64];
                        3'd3: resp_data_r <= refill_line[127:96];
                        3'd4: resp_data_r <= refill_line[159:128];
                        3'd5: resp_data_r <= refill_line[191:160];
                        3'd6: resp_data_r <= refill_line[223:192];
                        default: resp_data_r <= refill_line[255:224];
                    endcase
`ifdef CONFIG_EVTLOG
                    $display("ICACHE_REFILL_DONE addr=%h way=%0d", miss_addr, replace_way);
`endif
                    state <= IDLE;
                end
                
                default: state <= IDLE;
            endcase
        end
    end

    // 每次 miss 固定拉一条 8 beat 的 INCR burst，对应整条 32B cache line。
    assign axi_araddr  = miss_addr;
    assign axi_arlen   = 8'd7;
    assign axi_arsize  = 3'b010;
    assign axi_arburst = 2'b01;
    assign axi_arvalid = (state == AR_REQ);
    assign axi_rready  = (state == R_WAIT);

    // IDLE 才能接新请求；响应数据则由 hit 或 refill 完成时驱动。
    assign cpu_req_ready  = (state == IDLE);
    assign cpu_resp_valid = resp_valid_r;
    assign cpu_resp_data  = resp_data_r;

`ifdef BENCH
    reg stats_printed;
    always @(posedge clk) begin
        if (!resetn) begin
            stats_printed <= 1'b0;
        end else if ($root.Soc.HALT && !stats_printed) begin
            $display("ICACHE_STATS hit=%0d miss=%0d", hit_count, miss_count);
            stats_printed <= 1'b1;
        end
    end
`endif

endmodule
