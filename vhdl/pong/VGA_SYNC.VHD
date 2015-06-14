library IEEE;
use  IEEE.STD_LOGIC_1164.all;
use  IEEE.STD_LOGIC_ARITH.all;
use  IEEE.STD_LOGIC_UNSIGNED.all;
-- Module Generates Video Sync Signals for Video Montor Interface
-- RGB and Sync outputs tie directly to monitor conector pins
ENTITY VGA_SYNC IS
	PORT(	clock_48Mhz		: IN	STD_LOGIC;
			pixel_clock	: OUT	STD_LOGIC);
			
END VGA_SYNC;
ARCHITECTURE a OF VGA_SYNC IS
	SIGNAL pixel_clock_int : STD_LOGIC;

	COMPONENT video_PLL
	PORT
	(
		inclk0		: IN STD_LOGIC  := '0';
		c0			: OUT STD_LOGIC 
	);
end component;

BEGIN

-- PLL below is used to generate the pixel clock frequency
-- Uses UP 3's 48Mhz USB clock for PLL's input clock
video_PLL_inst : video_PLL PORT MAP (
		inclk0	 => Clock_48Mhz,
		c0	 => pixel_clock_int
	);


-- output pixel clock and video on for external user logic
pixel_clock <= pixel_clock_int;


END a;
