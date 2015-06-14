		-- control module (implements MIPS control unit)
LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.STD_LOGIC_ARITH.ALL;
USE IEEE.STD_LOGIC_SIGNED.ALL;

ENTITY control IS
   PORT( 	
	SIGNAL Opcode 		: IN 	STD_LOGIC_VECTOR( 5 DOWNTO 0 );
	SIGNAL RegDst 		: OUT 	STD_LOGIC;
	SIGNAL ALUSrc 		: OUT 	STD_LOGIC;
	SIGNAL MemtoReg 	: OUT 	STD_LOGIC;
	SIGNAL RegWrite 	: OUT 	STD_LOGIC;
	SIGNAL D_RegWrite_out 	: OUT 	STD_LOGIC;
	SIGNAL DD_RegWrite_out 	: OUT 	STD_LOGIC;
	SIGNAL MemRead 		: OUT 	STD_LOGIC;
	SIGNAL MemWrite 	: OUT 	STD_LOGIC;
	SIGNAL Branch 		: OUT 	STD_LOGIC;
	SIGNAL ALUop 		: OUT 	STD_LOGIC_VECTOR( 1 DOWNTO 0 );
	SIGNAL clock, reset	: IN 	STD_LOGIC );

END control;

ARCHITECTURE behavior OF control IS

	SIGNAL  R_format, Lw, Sw, Beq 	: STD_LOGIC;
	SIGNAL 	D_ALUSrc				: STD_LOGIC; 
	SIGNAL 	D_ALUOp					: STD_LOGIC_VECTOR( 1 DOWNTO 0 );
	SIGNAL 	D_Branch, DD_Branch		: STD_LOGIC;
	SIGNAL 	D_MemRead, DD_MemRead	: STD_LOGIC;
	SIGNAL 	D_MemWrite, DD_MemWrite	: STD_LOGIC;
	SIGNAL 	D_MemtoReg, DD_MemtoReg, DDD_MemtoReg	: STD_LOGIC;
	SIGNAL 	D_RegWrite, DD_RegWrite, DDD_RegWrite	: STD_LOGIC;	

BEGIN           
				-- Code to generate control signals using opcode bits
	R_format 	<=  '1'  WHEN  Opcode = "000000"  ELSE '0';
	Lw          <=  '1'  WHEN  Opcode = "100011"  ELSE '0';
 	Sw          <=  '1'  WHEN  Opcode = "101011"  ELSE '0';
   	Beq         <=  '1'  WHEN  Opcode = "000100"  ELSE '0';
  	RegDst    	<=  R_format;
 	D_ALUSrc  	<=  Lw OR Sw;
	D_ALUOp( 1 ) 	<=  R_format;
	D_ALUOp( 0 ) 	<=  Beq; 
  	DD_MemRead 		<=  Lw;
   	DD_MemWrite 	<=  Sw; 
 	Branch      	<=  Beq;
	DDD_MemtoReg 	<=  Lw;
  	DDD_RegWrite 	<=  R_format OR Lw;
	D_RegWrite_out	<= 	D_RegWrite;
	DD_RegWrite_out	<= 	DD_RegWrite;

  	PROCESS
		BEGIN
			WAIT UNTIL clock'EVENT AND clock='1';
			IF reset = '1' THEN
				ALUop <= "00";
				ALUSrc <= '0';
				MemRead <= '0';
				D_MemRead <= '0';
				MemWrite <= '0';
				D_MemWrite <= '0';
				D_Branch <= '0';
				MemtoReg <= '0';
				D_MemtoReg <= '0';
				DD_MemtoReg <= '0';
				RegWrite <= '0';
				D_RegWrite <= '0';
				DD_RegWrite <= '0';
			ELSE
				ALUSrc <= D_ALUsrc;
				ALUOp <= D_ALUOp;
				MemRead <= D_MemRead;
				D_MemRead <= DD_MemRead;
				MemWrite <= D_MemWrite;
				D_MemWrite <= DD_MemWrite;
				MemtoReg <= D_MemtoReg;
				D_MemtoReg <= DD_MemtoReg;
				DD_MemtoReg <= DDD_MemtoReg;
				RegWrite <= D_RegWrite;
				D_RegWrite <= DD_RegWrite;
				DD_RegWrite <= DDD_RegWrite;
			END IF;
	END PROCESS;

   END behavior;


