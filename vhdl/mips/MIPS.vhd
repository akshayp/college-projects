				-- Top Level Structural Model for MIPS Processor Core
LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.STD_LOGIC_ARITH.ALL;

ENTITY MIPS IS

	PORT( reset, clock					: IN  STD_LOGIC; 
		-- Output important signals to pins for easy display in Simulator
		PC								: OUT STD_LOGIC_VECTOR( 9 DOWNTO 0 );
		Stall_out						: OUT STD_LOGIC;
		Address_V_out					: OUT STD_LOGIC;
		Data_Ready_out					: OUT STD_LOGIC;
		Memory_Addr_out					: OUT STD_LOGIC_VECTOR (7 DOWNTO 0);
		Mem_Inst_out					: OUT STD_LOGIC_VECTOR (31 DOWNTO 0);
		D_ALU_result_out				: OUT STD_LOGIC_VECTOR( 31 DOWNTO 0 );
		ALU_result_out, read_data_1_out, read_data_2_out, write_data_out,	
     	Instruction_out					: OUT 	STD_LOGIC_VECTOR( 31 DOWNTO 0 );
		Branch_out, Zero_out, Memwrite_out, 
		D_RegisterWriteOut					: OUT 	STD_LOGIC;
		DD_RegisterWriteOut				: OUT 	STD_LOGIC;
		Regwrite_out					: OUT 	STD_LOGIC ;
		WriteRegOut						: OUT STD_LOGIC_VECTOR (4 DOWNTO 0);
		D_WriteRegOut						: OUT STD_LOGIC_VECTOR (4 DOWNTO 0);
		Add_Result_Out					:OUT 	STD_LOGIC_VECTOR( 7 DOWNTO 0 );
		DD_WriteRegOut						: OUT STD_LOGIC_VECTOR (4 DOWNTO 0));
		END 	MIPS;

ARCHITECTURE structure OF MIPS IS

	COMPONENT Ifetch
   	     PORT(	Instruction			: OUT 	STD_LOGIC_VECTOR( 31 DOWNTO 0 );
        		PC_plus_4_out 		: OUT  	STD_LOGIC_VECTOR( 9 DOWNTO 0 );
        		Add_result 			: IN 	STD_LOGIC_VECTOR( 7 DOWNTO 0 );
        		Branch 				: IN 	STD_LOGIC;
        		Zero 				: IN 	STD_LOGIC;
        		PC_out 				: OUT 	STD_LOGIC_VECTOR( 9 DOWNTO 0 );
        		clock,reset 		: IN 	STD_LOGIC );
	END COMPONENT; 

	COMPONENT Idecode
 	     PORT(	read_data_1 		: OUT 	STD_LOGIC_VECTOR( 31 DOWNTO 0 );
        		read_data_2_ex 		: OUT 	STD_LOGIC_VECTOR( 31 DOWNTO 0 );
        		read_data_2_mem 		: OUT 	STD_LOGIC_VECTOR( 31 DOWNTO 0 );
        		Instruction 		: IN 	STD_LOGIC_VECTOR( 31 DOWNTO 0 );
        		read_data 			: IN 	STD_LOGIC_VECTOR( 31 DOWNTO 0 );
        		RegWrite, MemtoReg 	: IN 	STD_LOGIC;
        		RegDst 				: IN 	STD_LOGIC;
				PC_plus_4 			: IN 	STD_LOGIC_VECTOR( 9 DOWNTO 0 );
 				RegWrite_mem	: IN STD_LOGIC;
 				RegWrite_ex	: IN STD_LOGIC;
				Zero 			: OUT	STD_LOGIC;
				Add_Result 			: OUT	STD_LOGIC_VECTOR( 7 DOWNTO 0 );
				write_register_address_mem : IN STD_LOGIC_VECTOR( 4 DOWNTO 0 );
				write_register_address_wb  : IN STD_LOGIC_VECTOR( 4 DOWNTO 0 );
				write_register_address_ex  : IN STD_LOGIC_VECTOR( 4 DOWNTO 0 );
        		Sign_extend 		: OUT 	STD_LOGIC_VECTOR( 31 DOWNTO 0 );
               	ALU_Result_mem 			: IN	STD_LOGIC_VECTOR( 31 DOWNTO 0 );
               	ALU_Result_wb 			: IN	STD_LOGIC_VECTOR( 31 DOWNTO 0 );
               	ALU_Result_ex			: IN	STD_LOGIC_VECTOR( 31 DOWNTO 0 );
 				read_register_1_address_out : OUT STD_LOGIC_VECTOR( 4 DOWNTO 0 );
				read_register_2_address_out : OUT STD_LOGIC_VECTOR( 4 DOWNTO 0 );
				D_read_register_1_address_out : OUT STD_LOGIC_VECTOR( 4 DOWNTO 0 );
				D_read_register_2_address_out : OUT STD_LOGIC_VECTOR( 4 DOWNTO 0 );
				D_write_register_address_out : OUT STD_LOGIC_VECTOR( 4 DOWNTO 0 );
				DD_write_register_address_out : OUT STD_LOGIC_VECTOR( 4 DOWNTO 0 );
				write_register_address_out : OUT STD_LOGIC_VECTOR( 4 DOWNTO 0 );
				write_data_out : OUT STD_LOGIC_VECTOR( 31 DOWNTO 0 );
       			clock, reset		: IN 	STD_LOGIC );
	END COMPONENT;

	COMPONENT control
	     PORT( 	Opcode 				: IN 	STD_LOGIC_VECTOR( 5 DOWNTO 0 );
             	RegDst 				: OUT 	STD_LOGIC;
             	ALUSrc 				: OUT 	STD_LOGIC;
             	MemtoReg 			: OUT 	STD_LOGIC;
             	RegWrite 			: OUT 	STD_LOGIC;
             	D_RegWrite_out		: OUT 	STD_LOGIC;
             	DD_RegWrite_out		: OUT 	STD_LOGIC;
             	MemRead 			: OUT 	STD_LOGIC;
             	MemWrite 			: OUT 	STD_LOGIC;
             	Branch 				: OUT 	STD_LOGIC;
             	ALUop 				: OUT 	STD_LOGIC_VECTOR( 1 DOWNTO 0 );
             	clock, reset		: IN 	STD_LOGIC );
	END COMPONENT;

	COMPONENT  Execute
   	     PORT(	Read_data_1 		: IN 	STD_LOGIC_VECTOR( 31 DOWNTO 0 );
                Read_data_2 		: IN 	STD_LOGIC_VECTOR( 31 DOWNTO 0 );
               	Sign_Extend 		: IN 	STD_LOGIC_VECTOR( 31 DOWNTO 0 );
               	Function_opcode		: IN 	STD_LOGIC_VECTOR( 5 DOWNTO 0 );
               	ALUOp 				: IN 	STD_LOGIC_VECTOR( 1 DOWNTO 0 );
               	ALUSrc 				: IN 	STD_LOGIC;
               	ALU_Result_mem 			: OUT	STD_LOGIC_VECTOR( 31 DOWNTO 0 );
               	ALU_Result_wb 			: OUT	STD_LOGIC_VECTOR( 31 DOWNTO 0 );
               	ALU_Result_ex 			: OUT	STD_LOGIC_VECTOR( 31 DOWNTO 0 );
               	PC_plus_4 			: IN 	STD_LOGIC_VECTOR( 9 DOWNTO 0 );
 				read_register_1_address    : IN STD_LOGIC_VECTOR( 4 DOWNTO 0 );
				read_register_2_address    : IN STD_LOGIC_VECTOR( 4 DOWNTO 0 );
				write_data		: IN STD_LOGIC_VECTOR( 31 DOWNTO 0 );
				RegWrite		: IN STD_LOGIC;
 				RegWrite_mem	: IN STD_LOGIC;
             	clock, reset	: IN STD_LOGIC );
	END COMPONENT;


	COMPONENT dmemory
	     PORT(	read_data 			: OUT 	STD_LOGIC_VECTOR( 31 DOWNTO 0 );
        		address 			: IN 	STD_LOGIC_VECTOR( 7 DOWNTO 0 );
        		write_data 			: IN 	STD_LOGIC_VECTOR( 31 DOWNTO 0 );
        		MemRead, Memwrite 	: IN 	STD_LOGIC;
        		Clock,reset			: IN 	STD_LOGIC );
	END COMPONENT;

					-- declare signals used to connect VHDL components
	SIGNAL PC_plus_4 		: STD_LOGIC_VECTOR( 9 DOWNTO 0 );
	SIGNAL read_data_1 		: STD_LOGIC_VECTOR( 31 DOWNTO 0 );
	SIGNAL read_data_2_ex 		: STD_LOGIC_VECTOR( 31 DOWNTO 0 );
	SIGNAL read_data_2_mem 		: STD_LOGIC_VECTOR( 31 DOWNTO 0 );
	SIGNAL Sign_Extend 		: STD_LOGIC_VECTOR( 31 DOWNTO 0 );
	SIGNAL Add_result 		: STD_LOGIC_VECTOR( 7 DOWNTO 0 );
	SIGNAL ALU_result_mem 		: STD_LOGIC_VECTOR( 31 DOWNTO 0 );
	SIGNAL ALU_result_wb 		: STD_LOGIC_VECTOR( 31 DOWNTO 0 );
	SIGNAL ALU_result_ex 		: STD_LOGIC_VECTOR( 31 DOWNTO 0 );
	SIGNAL read_data 		: STD_LOGIC_VECTOR( 31 DOWNTO 0 );
	SIGNAL ALUSrc 			: STD_LOGIC;
	SIGNAL Branch 			: STD_LOGIC;
	SIGNAL RegDst 			: STD_LOGIC;
	SIGNAL Regwrite 		: STD_LOGIC;
	SIGNAL Regwrite_mem		: STD_LOGIC;
	SIGNAL Regwrite_ex		: STD_LOGIC;
	SIGNAL Zero 			: STD_LOGIC;
	SIGNAL MemWrite 		: STD_LOGIC;
	SIGNAL MemtoReg 		: STD_LOGIC;
	SIGNAL MemRead 			: STD_LOGIC;
	SIGNAL ALUop 			: STD_LOGIC_VECTOR(  1 DOWNTO 0 );
	SIGNAL Instruction		: STD_LOGIC_VECTOR( 31 DOWNTO 0 );
	SIGNAL Stall			: STD_LOGIC;
	SIGNAL Address_V		: STD_LOGIC;
	SIGNAL Data_Ready		: STD_LOGIC;
	SIGNAL Memory_Addr		: STD_LOGIC_VECTOR (7 DOWNTO 0);
	SIGNAL Mem_Inst			: STD_LOGIC_VECTOR (31 DOWNTO 0);
	SIGNAL read_register_1_address : STD_LOGIC_VECTOR( 4 DOWNTO 0 );
	SIGNAL read_register_2_address : STD_LOGIC_VECTOR( 4 DOWNTO 0 );
	SIGNAL D_read_register_1_address : STD_LOGIC_VECTOR( 4 DOWNTO 0 );
	SIGNAL D_read_register_2_address : STD_LOGIC_VECTOR( 4 DOWNTO 0 );
	SIGNAL write_register_address_ex : STD_LOGIC_VECTOR( 4 DOWNTO 0 );
	SIGNAL write_register_address_mem : STD_LOGIC_VECTOR( 4 DOWNTO 0 );
	SIGNAL write_register_address_wb : STD_LOGIC_VECTOR( 4 DOWNTO 0 );
	SIGNAL write_data	: STD_LOGIC_VECTOR( 31 DOWNTO 0 );

BEGIN
					-- copy important signals to output pins for easy 
					-- display in Simulator
   Instruction_out 	<= Instruction;
   ALU_result_out 	<= ALU_result_mem;
   D_ALU_result_out <= ALU_result_wb;
   read_data_1_out 	<= read_data_1;
   read_data_2_out 	<= read_data_2_ex;
   write_data_out  	<= read_data WHEN MemtoReg = '1' ELSE ALU_result_wb;
   Branch_out 		<= Branch;
   Zero_out 		<= Zero;
   RegWrite_out 	<= RegWrite;
   MemWrite_out 	<= MemWrite;
   Stall_out		<= Stall;	
   Address_V_out	<= Address_V;
   Data_Ready_out	<= Data_Ready;
   Memory_Addr_out	<= Memory_Addr;
   Mem_Inst_out		<= Mem_Inst;
   D_RegisterWriteOut	<= RegWrite_mem;
   DD_RegisterWriteOut  <= RegWrite_ex;
	WriteRegOut		<=	write_register_address_wb;
	D_WriteRegOut		<=	write_register_address_mem;
	DD_WriteRegOut		<=	write_register_address_ex;
	Add_result_out	<= Add_Result;

					-- connect the 5 MIPS components   
  IFE : Ifetch
	PORT MAP (	Instruction 	=> Instruction,
    	    	PC_plus_4_out 	=> PC_plus_4,
				Add_result 		=> Add_result,
				Branch 			=> Branch,
				Zero 			=> Zero,
				PC_out 			=> PC,        		
				clock 			=> clock,  
				reset 			=> reset );

   ID : Idecode
   	PORT MAP (	read_data_1 	=> read_data_1,
        		read_data_2_ex 	=> read_data_2_ex,
	       		read_data_2_mem	=> read_data_2_mem,
				PC_plus_4 		=> PC_plus_4,
        		Instruction 	=> Instruction,
        		read_data 		=> read_data,
				RegWrite 		=> RegWrite,
				RegWrite_mem	=> RegWrite_mem,
				RegWrite_ex		=> RegWrite_ex,
				MemtoReg 		=> MemtoReg,
				RegDst 			=> RegDst,
				Zero 			=> Zero,
				Sign_extend 	=> Sign_extend,
				ALU_result_mem  => ALU_result_mem,
				ALU_result_wb	=> ALU_result_wb,
				ALU_result_ex	=> ALU_result_ex,
				Add_result => Add_result,
				write_register_address_ex => write_register_address_ex,
				write_register_address_mem => write_register_address_mem,
				write_register_address_wb  => write_register_address_wb,
				read_register_1_address_out => read_register_1_address,
				read_register_2_address_out => read_register_2_address,
				D_read_register_1_address_out => D_read_register_1_address,
				D_read_register_2_address_out => D_read_register_2_address,
				DD_write_register_address_out => write_register_address_ex,
				D_write_register_address_out => write_register_address_mem,
				write_register_address_out => write_register_address_wb,
				write_data_out	=> write_data,
        		clock 			=> clock,  
				reset 			=> reset );


   CTL:   control
	PORT MAP ( 	Opcode 			=> Instruction( 31 DOWNTO 26 ),
				RegDst 			=> RegDst,
				ALUSrc 			=> ALUSrc,
				MemtoReg 		=> MemtoReg,
				RegWrite 		=> RegWrite,
				D_RegWrite_out	=> RegWrite_mem,
				DD_RegWrite_out	=> RegWrite_ex,
				MemRead 		=> MemRead,
				MemWrite 		=> MemWrite,
				Branch 			=> Branch,
				ALUop 			=> ALUop,
                clock 			=> clock,
				reset 			=> reset );

   EXE:  Execute
   	PORT MAP (	Read_data_1 	=> read_data_1,
             	Read_data_2 	=> read_data_2_ex,
				Sign_extend 	=> Sign_extend,
                Function_opcode	=> Sign_extend( 5 DOWNTO 0 ),
				ALUOp 			=> ALUop,
				ALUSrc 			=> ALUSrc,
                ALU_Result_mem		=> ALU_Result_mem,
                ALU_Result_wb		=> ALU_Result_wb,
                ALU_Result_ex		=> ALU_Result_ex,
				PC_plus_4		=> PC_plus_4,
                read_register_1_address => read_register_1_address,
                read_register_2_address => read_register_2_address,
				write_data 		=> write_data,
				RegWrite		=> RegWrite,
				RegWrite_mem	=> RegWrite_mem,
				Clock			=> clock,
				Reset			=> reset );

   MEM:  dmemory
	PORT MAP (	read_data 		=> read_data,
				address 		=> ALU_Result_mem (7 DOWNTO 0),
				write_data 		=> read_data_2_mem,
				MemRead 		=> MemRead, 
				Memwrite 		=> MemWrite, 
                clock 			=> clock,  
				reset 			=> reset );
END structure;

