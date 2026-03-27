import sys

def convert_hex_to_coe(hex_file, coe_file):
    try:
        with open(hex_file, 'r') as f_in, open(coe_file, 'w') as f_out:
            f_out.write("memory_initialization_radix=16;\n")
            f_out.write("memory_initialization_vector=\n")
            
            lines = f_in.readlines()
            for i, line in enumerate(lines):
                line = line.strip()
                if line.startswith('@'):
                    continue # Ignore address markers for simple COE
                if not line:
                    continue
                    
                if i == len(lines) - 1 or (i == len(lines) - 2 and not lines[-1].strip()):
                    f_out.write(line + ";\n")
                else:
                    f_out.write(line + ",\n")
        print(f"Successfully converted {hex_file} to {coe_file}")
    except Exception as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python hex2coe.py <input.hex> <output.coe>")
    else:
        convert_hex_to_coe(sys.argv[1], sys.argv[2])
