`timescale 1ns / 1ps

module tb_icache();

    reg clk;
    reg resetn;
    
    // 这一组信号模拟处理器前端发起取指请求。
    reg         cpu_req_valid;
    reg  [31:0] cpu_req_addr;
    wire        cpu_req_ready;
    wire [31:0] cpu_resp_data;
    wire        cpu_resp_valid;

    // 这一组信号连接到 DDR 行为模型，模拟 cache miss 后的回填过程。
    wire [31:0] axi_araddr;
    wire [7:0]  axi_arlen;
    wire [2:0]  axi_arsize;
    wire [1:0]  axi_arburst;
    wire        axi_arvalid;
    wire        axi_arready;
    
    wire [31:0] axi_rdata;
    wire [1:0]  axi_rresp;
    wire        axi_rlast;
    wire        axi_rvalid;
    wire        axi_rready;

    // 被测对象是指令缓存本体。
    icache u_icache (
        .clk(clk),
        .resetn(resetn),
        .cpu_req_valid(cpu_req_valid),
        .cpu_req_en(1'b1),
        .cpu_req_addr(cpu_req_addr),
        .cpu_req_ready(cpu_req_ready),
        .cpu_resp_data(cpu_resp_data),
        .cpu_resp_valid(cpu_resp_valid),
        .axi_araddr(axi_araddr),
        .axi_arlen(axi_arlen),
        .axi_arsize(axi_arsize),
        .axi_arburst(axi_arburst),
        .axi_arvalid(axi_arvalid),
        .axi_arready(axi_arready),
        .axi_rdata(axi_rdata),
        .axi_rresp(axi_rresp),
        .axi_rlast(axi_rlast),
        .axi_rvalid(axi_rvalid),
        .axi_rready(axi_rready)
    );

    // 外部内存端由一个简单 AXI 只读模型提供。
    axi_ddr_model #(
        .MEM_SIZE(1024 * 1024)
    ) u_axi_ddr (
        .clk(clk),
        .resetn(resetn),
        .axi_araddr(axi_araddr),
        .axi_arlen(axi_arlen),
        .axi_arsize(axi_arsize),
        .axi_arburst(axi_arburst),
        .axi_arvalid(axi_arvalid),
        .axi_arready(axi_arready),
        .axi_rdata(axi_rdata),
        .axi_rresp(axi_rresp),
        .axi_rlast(axi_rlast),
        .axi_rvalid(axi_rvalid),
        .axi_rready(axi_rready)
    );

    initial begin
        clk = 0;
        // 10ns 周期时钟，便于观察状态机推进。
        forever #5 clk = ~clk;
    end

    initial begin
        // 先把输入拉成空闲状态，再释放复位开始测试。
        resetn = 0;
        cpu_req_valid = 0;
        cpu_req_addr = 0;

        #20 resetn = 1;
        #10;

        // 用第一条 cache line 验证首次访问 miss、后续同 line 访问 hit 的基本行为。
        $display("--- Test Case 1: First Access (Miss) ---");
        cpu_req_valid = 1;
        cpu_req_addr = 32'h0000_0000;
        
        wait(cpu_resp_valid == 1);
        $display("Time: %0t, Addr: %h, Data: %h (Expected: 00000000)", $time, cpu_req_addr, cpu_resp_data);
        
        @(posedge clk);
        
        $display("--- Test Case 2: Second Access (Hit) ---");
        cpu_req_addr = 32'h0000_0004;
        #1;
        if (cpu_resp_valid) begin
            $display("Time: %0t, Addr: %h, Data: %h (Expected: 00000001) - HIT!", $time, cpu_req_addr, cpu_resp_data);
        end else begin
            $display("Time: %0t, ERROR: Expected hit but got miss!", $time);
        end
        @(posedge clk);

        $display("--- Test Case 3: Third Access (Hit) ---");
        cpu_req_addr = 32'h0000_0008;
        #1;
        if (cpu_resp_valid) begin
            $display("Time: %0t, Addr: %h, Data: %h (Expected: 00000002) - HIT!", $time, cpu_req_addr, cpu_resp_data);
        end else begin
            $display("Time: %0t, ERROR: Expected hit but got miss!", $time);
        end
        @(posedge clk);

        // 跨到下一条 cache line，确认新的 line 会触发一次独立回填。
        $display("--- Test Case 4: Access another line (Miss) ---");
        cpu_req_addr = 32'h0000_0020;
        #1;
        wait(cpu_resp_valid == 1);
        $display("Time: %0t, Addr: %h, Data: %h (Expected: 00000008)", $time, cpu_req_addr, cpu_resp_data);
        @(posedge clk);
        
        // 回到第一条 line，应该仍然命中。
        $display("--- Test Case 5: Return to first line (Hit) ---");
        cpu_req_addr = 32'h0000_0000;
        #1;
        if (cpu_resp_valid) begin
            $display("Time: %0t, Addr: %h, Data: %h - HIT!", $time, cpu_req_addr, cpu_resp_data);
        end else begin
            $display("Time: %0t, ERROR: Expected hit but got miss!", $time);
        end
        @(posedge clk);

        // 接下来构造同一组中的三条不同 tag，验证 2 路组相联和 LRU 替换策略。
        $display("--- Test Case 6: Access 2nd line at same index (Tag 1, Index 0) ---");
        cpu_req_addr = 32'h0000_1000;
        #1;
        wait(cpu_resp_valid == 1);
        $display("Time: %0t, Addr: %h, Data: %h (Miss, filled way 1)", $time, cpu_req_addr, cpu_resp_data);
        @(posedge clk);

        $display("--- Test Case 7: Access 1st line again (Hit, way 0 becomes MRU, way 1 is LRU) ---");
        cpu_req_addr = 32'h0000_0000;
        #1;
        if (cpu_resp_valid) begin
            $display("Time: %0t, Addr: %h, Data: %h - HIT!", $time, cpu_req_addr, cpu_resp_data);
        end else begin
            $display("Time: %0t, ERROR: Expected hit but got miss!", $time);
        end
        @(posedge clk);

        $display("--- Test Case 8: Access 3rd line at same index (Tag 2, Index 0) ---");
        cpu_req_addr = 32'h0000_2000;
        #1;
        wait(cpu_resp_valid == 1);
        $display("Time: %0t, Addr: %h, Data: %h (Miss, should replace way 1)", $time, cpu_req_addr, cpu_resp_data);
        @(posedge clk);

        $display("--- Test Case 9: Access 1st line again (Should hit, way 0 was not evicted) ---");
        cpu_req_addr = 32'h0000_0000;
        #1;
        if (cpu_resp_valid) begin
            $display("Time: %0t, Addr: %h, Data: %h - HIT!", $time, cpu_req_addr, cpu_resp_data);
        end else begin
            $display("Time: %0t, ERROR: Expected hit but got miss!", $time);
        end
        @(posedge clk);

        $display("--- Test Case 10: Access 2nd line again (Should miss, way 1 was evicted) ---");
        cpu_req_addr = 32'h0000_1000;
        #1;
        if (!cpu_resp_valid) begin
            $display("Time: %0t, Addr: %h - MISS (Correct, it was evicted by LRU)", $time, cpu_req_addr);
            wait(cpu_resp_valid == 1);
            $display("Time: %0t, Addr: %h, Data: %h (Miss handled)", $time, cpu_req_addr, cpu_resp_data);
        end else begin
            $display("Time: %0t, ERROR: Expected miss but got hit!", $time);
        end
        @(posedge clk);

        cpu_req_valid = 0;
        #50;
        $display("All tests completed.");
        $finish;
    end

endmodule
