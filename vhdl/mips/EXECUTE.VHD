--  Execute module (implements the data ALU and Branch Address Adder  
--  for the MIPS computer)
LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.STD_LOGIC_ARITH.ALL;
USE IEEE.STD_LOGIC_SIGNED.ALL;

ENTITY  Execute IS
	PORT(	Read_data_1 	: IN 	STD_LOGIC_VECTOR( 31 DOWNTO 0 );
			Read_data_2 	: IN 	STD_LOGIC_VECTOR( 31 DOWNTO 0 );
			Sign_extend 	: IN 	STD_LOGIC_VECTOR( 31 DOWNTO 0 );
			Function_opcode : IN 	STD_LOGIC_VECTOR( 5 DOWNTO 0 );
			ALUOp 			: IN 	STD_LOGIC_VECTOR( 1 DOWNTO 0 );
			ALUSrc 			: IN 	STD_LOGIC;
			ALU_Result_mem 		: OUT	STD_LOGIC_VECTOR( 31 DOWNTO 0 );
			ALU_Result_wb 		: OUT	STD_LOGIC_VECTOR( 31 DOWNTO 0 );
			ALU_Result_ex 		: OUT	STD_LOGIC_VECTOR( 31 DOWNTO 0 );
			PC_plus_4 		: IN 	STD_LOGIC_VECTOR( 9 DOWNTO 0 );
			read_register_1_address    : IN STD_LOGIC_VECTOR( 4 DOWNTO 0 );
			read_register_2_address    : IN STD_LOGIC_VECTOR( 4 DOWNTO 0 );
			write_data		: IN STD_LOGIC_VECTOR( 31 DOWNTO 0 );
			RegWrite		: IN STD_LOGIC;
			RegWrite_mem	: IN STD_LOGIC;
			clock, reset	: IN 	STD_LOGIC );
END Execute;

ARCHITECTURE behavior OF Execute IS
SIGNAL Ainput, Binput 		: STD_LOGIC_VECTOR( 31 DOWNTO 0 );
SIGNAL Binput_mux			: STD_LOGIC_VECTOR( 31 DOWNTO 0 );
SIGNAL ALU_output_mux		: STD_LOGIC_VECTOR( 31 DOWNTO 0 );
SIGNAL Branch_Add 			: STD_LOGIC_VECTOR( 7 DOWNTO 0 );
SIGNAL ALU_ctl				: STD_LOGIC_VECTOR( 2 DOWNTO 0 );
SIGNAL D_ALU_Result, DD_ALU_Result	: STD_LOGIC_VECTOR( 31 DOWNTO 0 );

BEGIN
		Ainput <= Read_data_1;
						-- ALU input mux
	Binput_mux <= Read_data_2 
		WHEN ( ALUSrc = '0' ) 
  		ELSE  Sign_extend( 31 DOWNTO 0 );

	Binput <= Binput_mux;

						-- Generate ALU control bits
	ALU_ctl( 0 ) <= ( Function_opcode( 0 ) OR Function_opcode( 3 ) ) AND ALUOp(1 );
	ALU_ctl( 1 ) <= ( NOT Function_opcode( 2 ) ) OR (NOT ALUOp( 1 ) );
	ALU_ctl( 2 ) <= ( Function_opcode( 1 ) AND ALUOp( 1 )) OR ALUOp( 0 );
						-- Generate Zero Flag
	-- Select ALU output        
	DD_ALU_result <= X"0000000" & B"000"  & ALU_output_mux( 31 ) 
		WHEN  ALU_ctl = "111" 
		ELSE  	ALU_output_mux( 31 DOWNTO 0 );
						-- Adder to compute Branch Address	

PROCESS ( ALU_ctl, Ainput, Binput )
	BEGIN
					-- Select ALU operation
 	CASE ALU_ctl IS
						-- ALU performs ALUresult = A_input AND B_input
		WHEN "000" 	=>	ALU_output_mux 	<= Ainput AND Binput; 
						-- ALU performs ALUresult = A_input OR B_input
     	WHEN "001" 	=>	ALU_output_mux 	<= Ainput OR Binput;
						-- ALU performs ALUresult = A_input + B_input
	 	WHEN "010" 	=>	ALU_output_mux 	<= Ainput + Binput;
						-- ALU performs ?
 	 	WHEN "011" 	=>	ALU_output_mux <= X"00000000";
						-- ALU performs ?
 	 	WHEN "100" 	=>	ALU_output_mux 	<= X"00000000";
						-- ALU performs ?
 	 	WHEN "101" 	=>	ALU_output_mux 	<= X"00000000";
						-- ALU performs ALUresult = A_input -B_input
 	 	WHEN "110" 	=>	ALU_output_mux 	<= Ainput - Binput;
						-- ALU performs SLT
  	 	WHEN "111" 	=>	ALU_output_mux 	<= Ainput - Binput ;
 	 	WHEN OTHERS	=>	ALU_output_mux 	<= X"00000000" ;
  	END CASE;
  END PROCESS;


PROCESS
	BEGIN
		WAIT UNTIL clock = '0'; 
		ALU_Result_ex <= DD_ALU_Result;
	END PROCESS;


PROCESS
	BEGIN
		WAIT UNTIL rising_edge(clock);
		ALU_Result_mem <= DD_ALU_Result;
		D_ALU_Result <= DD_ALU_Result;
		ALU_Result_wb <= D_ALU_Result;
	END PROCESS;

END behavior;

