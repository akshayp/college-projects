
       -- THIS FILE CONTAINS A FIX MADE ON 9/19/2005.


                         --  Idecode module (implements the register file for
LIBRARY IEEE;            -- the MIPS computer)
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.STD_LOGIC_ARITH.ALL;
USE IEEE.STD_LOGIC_UNSIGNED.ALL;

ENTITY Idecode IS
	  PORT(	read_data_1	: OUT 	STD_LOGIC_VECTOR( 31 DOWNTO 0 );
			read_data_2_ex	: OUT 	STD_LOGIC_VECTOR( 31 DOWNTO 0 );
			read_data_2_mem	: OUT 	STD_LOGIC_VECTOR( 31 DOWNTO 0 );
			Instruction : IN 	STD_LOGIC_VECTOR( 31 DOWNTO 0 );
			read_data 	: IN 	STD_LOGIC_VECTOR( 31 DOWNTO 0 );
			ALU_result	: IN 	STD_LOGIC_VECTOR( 31 DOWNTO 0 );
			RegWrite 	: IN 	STD_LOGIC;
			MemtoReg 	: IN 	STD_LOGIC;
			RegDst 		: IN 	STD_LOGIC;
			RegWrite_mem	: IN STD_LOGIC;
			RegWrite_ex	: IN STD_LOGIC;
			Sign_extend : OUT 	STD_LOGIC_VECTOR( 31 DOWNTO 0 );
			ALU_Result_mem 		: IN	STD_LOGIC_VECTOR( 31 DOWNTO 0 );
			ALU_Result_wb 		: IN	STD_LOGIC_VECTOR( 31 DOWNTO 0 );
			ALU_Result_ex 		: IN	STD_LOGIC_VECTOR( 31 DOWNTO 0 );
			read_register_1_address_out : OUT STD_LOGIC_VECTOR( 4 DOWNTO 0 );
			read_register_2_address_out : OUT STD_LOGIC_VECTOR( 4 DOWNTO 0 );
			D_read_register_1_address_out : OUT STD_LOGIC_VECTOR( 4 DOWNTO 0 );
			D_read_register_2_address_out : OUT STD_LOGIC_VECTOR( 4 DOWNTO 0 );
			D_write_register_address_out : OUT STD_LOGIC_VECTOR( 4 DOWNTO 0 );
			DD_write_register_address_out : OUT STD_LOGIC_VECTOR( 4 DOWNTO 0 );
			write_register_address_out : OUT STD_LOGIC_VECTOR( 4 DOWNTO 0 );
			write_register_address_ex  : IN STD_LOGIC_VECTOR( 4 DOWNTO 0 );
			write_register_address_mem : IN STD_LOGIC_VECTOR( 4 DOWNTO 0 );
			write_register_address_wb  : IN STD_LOGIC_VECTOR( 4 DOWNTO 0 );
			write_data_out : OUT STD_LOGIC_VECTOR( 31 DOWNTO 0 );
			Zero 			: OUT	STD_LOGIC;
			Add_Result 		: OUT	STD_LOGIC_VECTOR( 7 DOWNTO 0 );
			PC_plus_4 		: IN 	STD_LOGIC_VECTOR( 9 DOWNTO 0 );
			
			clock,reset	: IN 	STD_LOGIC );
END Idecode;


ARCHITECTURE behavior OF Idecode IS
TYPE register_file IS ARRAY ( 0 TO 31 ) OF STD_LOGIC_VECTOR( 31 DOWNTO 0 );

	SIGNAL clkbar						: STD_LOGIC;
	SIGNAL register_array				: register_file;
	SIGNAL write_register_address 		: STD_LOGIC_VECTOR( 4 DOWNTO 0 );
	SIGNAL write_data					: STD_LOGIC_VECTOR( 31 DOWNTO 0 );
	SIGNAL read_register_1_address		: STD_LOGIC_VECTOR( 4 DOWNTO 0 );
	SIGNAL read_register_2_address		: STD_LOGIC_VECTOR( 4 DOWNTO 0 );
	SIGNAL write_register_address_1		: STD_LOGIC_VECTOR( 4 DOWNTO 0 );
	SIGNAL write_register_address_0		: STD_LOGIC_VECTOR( 4 DOWNTO 0 );
	SIGNAL Instruction_immediate_value	: STD_LOGIC_VECTOR( 15 DOWNTO 0 );
	SIGNAL D_read_data_1				: STD_LOGIC_VECTOR( 31 DOWNTO 0 );
	SIGNAL D_read_data_2, DD_read_data_2	: STD_LOGIC_VECTOR( 31 DOWNTO 0 );
	SIGNAL Forward_A, Forward_B : STD_LOGIC_VECTOR( 1 DOWNTO 0 );
	SIGNAL ALUResult, D_ALU_Result, DD_ALU_Result, DDD_ALU_Result	: STD_LOGIC_VECTOR( 31 DOWNTO 0 );
	
	SIGNAL D_Sign_extend				: STD_LOGIC_VECTOR( 31 DOWNTO 0 );
	SIGNAL D_write_register_address, DD_write_register_address, 
			DDD_write_register_address	: STD_LOGIC_VECTOR( 4 DOWNTO 0 );
	SIGNAL D_read_register_1_address, D_read_register_2_address :
			STD_LOGIC_VECTOR( 4 DOWNTO 0 );
	SIGNAL D_Add_Result			: STD_LOGIC_VECTOR( 7 DOWNTO 0 );		
	SIGNAL Branch_Add 			: STD_LOGIC_VECTOR( 7 DOWNTO 0 );		
	SIGNAL D_Zero				: STD_LOGIC;


BEGIN
	clkbar <= NOT clock;
	read_register_1_address_out <= read_register_1_address;
	read_register_2_address_out <= read_register_2_address;
	D_read_register_1_address_out <= D_read_register_1_address;
	D_read_register_2_address_out <= D_read_register_2_address;
	D_write_register_address_out <= D_write_register_address;
	DD_write_register_address_out <= DD_write_register_address;
	write_register_address_out <= write_register_address;
	write_data_out <= write_data;
	D_read_register_1_address 	<= Instruction( 25 DOWNTO 21 );
   	D_read_register_2_address 	<= Instruction( 20 DOWNTO 16 );
   	write_register_address_1	<= Instruction( 15 DOWNTO 11 );
   	write_register_address_0 	<= Instruction( 20 DOWNTO 16 );
   	Instruction_immediate_value <= Instruction( 15 DOWNTO 0 );

PROCESS (RegWrite, RegWrite_mem, write_register_address_mem, write_register_address_wb,
			 read_register_1_address, read_register_2_address)
	BEGIN
		IF RegWrite_ex = '1' AND write_register_address_ex /= "00000"
			AND write_register_address_ex = D_read_register_1_address THEN
			Forward_A <= "11";
		ELSIF RegWrite_mem = '1' AND write_register_address_mem /= "00000"
				AND write_register_address_mem = D_read_register_1_address THEN
			Forward_A <= "10";
		ELSIF RegWrite = '1' AND write_register_address_wb /= "00000"
			AND write_register_address_wb = D_read_register_1_address THEN
			Forward_A <= "01";
		ELSE Forward_A <= "00";
		END IF;
		
		IF RegWrite_ex = '1' AND write_register_address_ex /= "00000"
			AND write_register_address_ex = D_read_register_2_address THEN
			Forward_B <= "11";
		ELSIF RegWrite_mem = '1' AND write_register_address_mem /= "00000"
				AND write_register_address_mem = D_read_register_2_address THEN
			Forward_B <= "10";
		ELSIF RegWrite = '1' AND write_register_address_wb /= "00000"
			AND write_register_address_wb = D_read_register_2_address THEN
			Forward_B <= "01";
		ELSE Forward_B <= "00";
		END IF;
	END PROCESS;

PROCESS 
	BEGIN
		WAIT UNTIL clock = '0'; 
			DDD_ALU_Result <= ALU_Result_ex;
			DD_ALU_Result <= ALU_Result_mem;
			D_ALU_Result <= ALU_Result_wb;
 	END PROCESS;


					-- Read Register 1 Operation
	D_read_data_1 <= register_array( CONV_INTEGER( D_read_register_1_address ) ) WHEN Forward_A = "00" ELSE
				  DD_ALU_Result	WHEN Forward_A = "10" ELSE
				  D_ALU_Result	WHEN Forward_A = "01" ELSE
				  ALU_Result_ex;
					-- Read Register 2 Operation		 
	DD_read_data_2 <= register_array( CONV_INTEGER( D_read_register_2_address ) )	WHEN Forward_B = "00" ELSE
				  DD_ALU_Result	WHEN Forward_B = "10" ELSE
				  D_ALU_Result	WHEN Forward_B = "01" ELSE
				  ALU_Result_ex;		
					-- Mux for Register Write Address
						-- ALU input mux	

		
		
    	DDD_write_register_address <= write_register_address_1 
			WHEN RegDst = '1'  			ELSE write_register_address_0;
					-- Mux to bypass data memory for Rformat instructions
	write_data <= ALU_result_wb( 31 DOWNTO 0 ) 
			WHEN ( MemtoReg = '0' ) 	ELSE read_data;
					-- Sign Extend 16-bits to 32-bits
    	D_Sign_extend <= X"0000" & Instruction_immediate_value
		WHEN Instruction_immediate_value(15) = '0'
		ELSE	X"FFFF" & Instruction_immediate_value;
	
		Zero <= '1' 
		WHEN ( D_read_data_1 = DD_read_data_2  )
		ELSE '0';    
	
		
	Branch_Add	<= PC_plus_4( 9 DOWNTO 2 ) +  Instruction_immediate_value(7 DOWNTO 0) ;
	Add_result 	<= Branch_Add( 7 DOWNTO 0 );	

PROCESS 
	BEGIN
		WAIT UNTIL clock'EVENT AND clock = '1'; 
			read_data_1 <= D_read_data_1;
			read_data_2_ex <= DD_read_data_2;	
			D_read_data_2 <= DD_read_data_2;
			read_data_2_mem <= D_read_data_2;
			Sign_extend <= D_sign_extend;
			write_register_address <= D_write_register_address;
			D_write_register_address <= DD_write_register_address;
			DD_write_register_address <= DDD_write_register_address;
			read_register_1_address <= D_read_register_1_address;
			read_register_2_address <= D_read_register_2_address;
			--DD_ALU_Result <= ALU_Result_mem;
			--D_ALU_Result <= ALU_Result_wb;
 	END PROCESS;

PROCESS
	BEGIN
		WAIT UNTIL clkbar'EVENT AND clkbar = '1';
			IF reset = '1' THEN
					-- Initial register values on reset are register = reg#
					-- use loop to automatically generate reset logic 
					-- for all registers
				FOR i IN 0 TO 31 LOOP
					register_array(i) <= CONV_STD_LOGIC_VECTOR( i, 32 );
 				END LOOP;
					-- Write back to register - don't write to register 0
 			ELSIF RegWrite = '1' AND write_register_address /= 0 THEN
		    	register_array( CONV_INTEGER( write_register_address)) <= write_data;
			END IF;
	END PROCESS;

END behavior;


