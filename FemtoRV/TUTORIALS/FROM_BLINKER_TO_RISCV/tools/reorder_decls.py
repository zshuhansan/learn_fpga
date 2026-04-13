#!/usr/bin/env python3
import os, re, sys

script_dir = os.path.dirname(os.path.realpath(__file__))
project_root = os.path.abspath(os.path.join(script_dir, '..'))
BASE = os.path.join(project_root, 'Soc', 'src')
if not os.path.isdir(BASE):
    print('Soc/src not found at', BASE)
    sys.exit(1)

decl_start_re = re.compile(r'^\s*(wire|reg|localparam|parameter|integer|genvar)\b')
attr_re = re.compile(r'^\s*\(\*')
function_re = re.compile(r'^\s*function\b')
task_re = re.compile(r'^\s*task\b')
endfunction_re = re.compile(r'^\s*endfunction\b')
endtask_re = re.compile(r'^\s*endtask\b')

changed = []
for fn in sorted(os.listdir(BASE)):
    if not fn.endswith('.v'):
        continue
    path = os.path.join(BASE, fn)
    with open(path, 'r', encoding='utf-8') as f:
        text = f.read()

    m = re.search(r'\bmodule\b', text)
    if not m:
        continue
    mod_start = m.start()
    # find end of port list: first occurrence of ");" after module start
    port_end_idx = text.find(');', mod_start)
    if port_end_idx == -1:
        # can't find port-list end, skip
        continue
    port_end_idx += 2
    endmodule_idx = text.find('endmodule', port_end_idx)
    if endmodule_idx == -1:
        continue

    header = text[:port_end_idx]
    body = text[port_end_idx:endmodule_idx]
    footer = text[endmodule_idx:]

    lines = body.splitlines(keepends=True)
    decls = []
    rest = []
    i = 0
    pp_depth = 0
    while i < len(lines):
        line = lines[i]
        # track preprocessor conditional depth so we don't lift declarations out of `ifdef/`endif blocks
        lstr = line.lstrip()
        if lstr.startswith('`ifdef') or lstr.startswith('`ifndef'):
            pp_depth += 1
            rest.append(line)
            i += 1
            continue
        if lstr.startswith('`endif'):
            pp_depth = max(0, pp_depth - 1)
            rest.append(line)
            i += 1
            continue
        # skip empty lines
        if attr_re.match(line):
            # attribute: include attr and next line if declaration (capture multi-line decls)
            # do not lift declarations that are inside preprocessor conditionals
            if pp_depth == 0 and i+1 < len(lines) and decl_start_re.match(lines[i+1]):
                block = [line]
                i += 1
                block.append(lines[i])
                i += 1
                # if declaration is multi-line, keep consuming until a terminating ';' is found
                while i < len(lines) and ';' not in block[-1]:
                    block.append(lines[i])
                    i += 1
                decls.extend(block)
                continue
            else:
                rest.append(line)
                i += 1
                continue
        if function_re.match(line):
            # capture until endfunction
            block = [line]
            i += 1
            while i < len(lines):
                block.append(lines[i])
                if endfunction_re.match(lines[i]):
                    i += 1
                    break
                i += 1
            decls.extend(block)
            continue
        if task_re.match(line):
            block = [line]
            i += 1
            while i < len(lines):
                block.append(lines[i])
                if endtask_re.match(lines[i]):
                    i += 1
                    break
                i += 1
            decls.extend(block)
            continue
        if pp_depth == 0 and decl_start_re.match(line):
            # capture single- or multi-line declarations until terminating ';'
            block = [line]
            i += 1
            while i < len(lines) and ';' not in block[-1]:
                block.append(lines[i])
                i += 1
            decls.extend(block)
            continue
        # otherwise keep
        rest.append(line)
        i += 1

    # decide whether to rewrite: only if there's at least one decl and first non-empty line in body is not a decl
    first_nonblank = None
    for L in lines:
        if L.strip() == '':
            continue
        first_nonblank = L
        break
    need_rewrite = False
    if decls and first_nonblank and not decl_start_re.match(first_nonblank) and not attr_re.match(first_nonblank) and not function_re.match(first_nonblank) and not task_re.match(first_nonblank):
        need_rewrite = True

    if need_rewrite:
        new_body = []
        # ensure a separating newline
        if decls and (not decls[-1].endswith('\n')):
            decls = [d if d.endswith('\n') else d+'\n' for d in decls]
        new_body.extend(decls)
        new_body.append('\n')
        new_body.extend(rest)
        new_text = header + ''.join(new_body) + footer
        out_path = path + '.reordered'
        with open(out_path, 'w', encoding='utf-8') as fo:
            fo.write(new_text)
        changed.append((path, out_path))
        print('Reordered declarations in', path)

if not changed:
    print('No reorder needed')
else:
    print('\nFiles rewritten to .reordered:')
    for a,b in changed:
        print(a,'->',b)
