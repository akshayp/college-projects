--  Dmemory module (implements the data
-- memory for the MIPS computer)

LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.STD_LOGIC_ARITH.ALL;
USE IEEE.STD_LOGIC_SIGNED.ALL;
LIBRARY lpm;
USE lpm.lpm_components.ALL;

ENTITY dmemory IS
	PORT(	read_data 			: OUT 	STD_LOGIC_VECTOR( 7 DOWNTO 0 );
        		address 			: IN 	STD_LOGIC_VECTOR( 7 DOWNTO 0 );
        		write_data 			: IN 	STD_LOGIC_VECTOR( 7 DOWNTO 0 );
	   		MemRead, Memwrite 	: IN 	STD_LOGIC;
            		clock,reset			: IN 	STD_LOGIC );
END dmemory;

ARCHITECTURE behavior OF dmemory IS
SIGNAL lpm_write : STD_LOGIC;
BEGIN
	data_memory: lpm_ram_dq
	GENERIC MAP ( 
		lpm_widthad 	=> 	8,
		lpm_outdata 	=> 	"UNREGISTERED",
		lpm_indata 	=> 	"REGISTERED",
		lpm_address_control 	=> 	"UNREGISTERED",
				-- Reads in mif file for initial data memory values
		lpm_file 	=> 	"dmemory.mif",
		lpm_width 	=> 	8 )

      PORT MAP (
		data	=>	write_data, 	address 	=> 	address, 
		we 	=>	lpm_write,	inclock 	=> 	clock,      q  =>  read_data );
				-- delay lpm write enable to ensure stable address and data
		lpm_write <= memwrite AND ( NOT clock );
END behavior;

