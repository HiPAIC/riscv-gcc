;; HiPAIC DFA-based pipeline description for RISC-V targets.
;; This is mostly adapted from generic.md with minor edits and adds secret multiply.

(define_automaton "hipaic_0")
(define_cpu_unit "hipaic_0_alu" "hipaic_0")
(define_cpu_unit "hipaic_0_imuldiv" "hipaic_0")
(define_cpu_unit "hipaic_0_fdivsqrt" "hipaic_0")
(define_cpu_unit "hipaic_0_secret_mul" "hipaic_0")


(define_insn_reservation "hipaic_0_alu" 1
  (and (eq_attr "tune" "hipaic_0")
       (eq_attr "type" "unknown,const,arith,shift,slt,multi,auipc,nop,logical,move"))
  "hipaic_0_alu")

(define_insn_reservation "hipaic_0_load" 3
  (and (eq_attr "tune" "hipaic_0")
       (eq_attr "type" "load,fpload"))
  "hipaic_0_alu")

(define_insn_reservation "hipaic_0_store" 1
  (and (eq_attr "tune" "hipaic_0")
       (eq_attr "type" "store,fpstore"))
  "hipaic_0_alu")

(define_insn_reservation "hipaic_0_xfer" 3
  (and (eq_attr "tune" "hipaic_0")
       (eq_attr "type" "mfc,mtc,fcvt,fmove,fcmp"))
  "hipaic_0_alu")

(define_insn_reservation "hipaic_0_branch" 1
  (and (eq_attr "tune" "hipaic_0")
       (eq_attr "type" "branch,jump,call"))
  "hipaic_0_alu")

(define_insn_reservation "hipaic_0_idivsi" 34
  (and (eq_attr "tune" "hipaic_0")
       (and (eq_attr "type" "idiv")
	    (eq_attr "mode" "SI")))
  "hipaic_0_imuldiv*34")

(define_insn_reservation "hipaic_0_idivdi" 66
  (and (eq_attr "tune" "hipaic_0")
       (and (eq_attr "type" "idiv")
	    (eq_attr "mode" "DI")))
  "hipaic_0_imuldiv*66")

(define_insn_reservation "hipaic_0_fmul_single" 5
  (and (eq_attr "tune" "hipaic_0")
       (and (eq_attr "type" "fadd,fmul,fmadd")
	    (eq_attr "mode" "SF")))
  "hipaic_0_alu")

(define_insn_reservation "hipaic_0_fmul_double" 7
  (and (eq_attr "tune" "hipaic_0")
       (and (eq_attr "type" "fadd,fmul,fmadd")
	    (eq_attr "mode" "DF")))
  "hipaic_0_alu")

(define_insn_reservation "hipaic_0_fdiv" 20
  (and (eq_attr "tune" "hipaic_0")
       (eq_attr "type" "fdiv"))
  "hipaic_0_fdivsqrt*20")

(define_insn_reservation "hipaic_0_fsqrt" 25
  (and (eq_attr "tune" "hipaic_0")
       (eq_attr "type" "fsqrt"))
  "hipaic_0_fdivsqrt*25")

;; mul is implemented with only 1 cycle latency.
(define_insn_reservation "hipaic_0_imul" 1
  (and (eq_attr "tune" "hipaic_0")
       (eq_attr "type" "imul"))
  "hipaic_0_imuldiv")

;; GenNewRand / LoadOpX / GetNextRand are currently 1 cycle latency.
;; We do not refine the cpu_unit for now. If needed, we can define resources like RNG later. 
;; But now it does not change any instruction latency / conflict because we do not have multi-issue.
;; So we keep the DFA as simple as possible.
(define_insn_reservation "hipaic_0_secret_insns" 1
  (and (eq_attr "tune" "hipaic_0")
       (eq_attr "type" "secret_newrand,secret_opx"))
  "hipaic_0_alu")

;; secret multiply is implemented with 3 cycle latency.
;; This is 3 cycle so we don't want to reserve ALU. We must reserve a separate unit.
;; Otherwise it forces delay 3 cycles even if the next 2 instructions are not data dependent on this.
(define_insn_reservation "hipaic_0_secret_multiply" 3
  (and (eq_attr "tune" "hipaic_0")
       (eq_attr "type" "secret_mul"))
  "hipaic_0_secret_mul*3")
