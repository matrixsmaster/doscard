#ifndef VM86MACRO_H_
#define VM86MACRO_H_


// Helper macros

// Return memory-mapped register location (offset into mem array) for register #reg_id
#define GET_REG_ADDR(reg_id) (REGS_BASE + (i_w ? 2 * reg_id : (2 * reg_id + reg_id / 4) & 7))

// Returns number of top bit in operand (i.e. 8 for 8-bit operands, 16 for 16-bit operands)
#define TOP_BIT 8*(i_w + 1)

// Opcode execution unit helpers
#define OPCODE 			break; \
						case
#define OPCODE_CHAIN 	case

#define ADC_SBB_MACRO(a) OP(a##= regs8[FLAG_CF] +), \
						 set_CF((regs8[FLAG_CF] && ((unsigned)op_result == op_dest)) || (a op_result < a(int)op_dest)), \
						 set_AF_OF_arith()

// Execute arithmetic/logic operations in emulator memory/registers
#define R_M_OP(dest,op,src) (i_w ? op_dest = CAST(uint16_t,dest), op_result = CAST(uint16_t,dest) op (op_source = CAST(uint16_t,src)) \
								 : (op_dest = dest, op_result = dest op (op_source = CAST(uint8_t,src))))

#define MEM_OP(dest,op,src) R_M_OP(mem[dest],op,mem[src])
#define OP(op) MEM_OP(op_to_addr,op,op_from_addr)

// Increment or decrement a register #reg_id (usually SI or DI), depending on direction flag and operand size (given by i_w)
#define INDEX_INC(reg_id) (regs16[reg_id] -= (2 * regs8[FLAG_DF] - 1)*(i_w + 1))

// Helpers for stack operations
#define R_M_PUSH(a) (i_w = 1, R_M_OP(mem[SEGREG(REG_SS, REG_SP, --)], =, a))
#define R_M_POP(a) (i_w = 1, regs16[REG_SP] += 2, R_M_OP(a, =, mem[SEGREG(REG_SS, REG_SP, -2+)]))

// Convert segment:offset to linear address in emulator memory space
#define SEGREG(reg_seg,reg_ofs,op) 16 * regs16[reg_seg] + (uint16_t)(op regs16[reg_ofs])

// Returns sign bit of an 8-bit or 16-bit operand
#define SIGN_OF(a) (1 & (i_w ? CAST(int16_t,a) : a) >> (TOP_BIT - 1))

// Reinterpretation cast
#define CAST(a,b) *(a*)&b
//#define CAST(T,X) (*reinterpret_cast<T*>(&(X)))


#endif /* VM86MACRO_H_ */
