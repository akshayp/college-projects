LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.STD_LOGIC_ARITH.ALL;

ENTITY bat IS
	PORT ( clr, CLKBAT     			: IN STD_LOGIC;
			scan_code				: IN	STD_LOGIC_VECTOR(7 DOWNTO 0);
			scan_ready 	: IN std_logic; 
		 	read 					: OUT std_logic;
		   	paddle_1, paddle_2 		: OUT INTEGER	RANGE 0 to 48);
		
END bat;


ARCHITECTURE position OF bat IS

TYPE STATE_TYPE IS (wait_ready, read_data, read_low);
SIGNAL state: STATE_TYPE;
SIGNAL clock_enable :Std_logic;
signal a, b, c, d ,e, f, g, h : std_logic_vector(15 downto 0);

	BEGIN
	
	a <= "0001101100011011";
	b <= "1111000000011011";
	c <= "0001110100011101";
	d <= "1111000000011101";
	e <= "0100110001001100";
	f <= "1111000001001100";
	g <= "0100110101001101";
	h <= "1111000001001101";
	 	PROCESS(scan_ready, CLR,CLKBAT, clock_enable)
	 	   VARIABLE pdl_1, pdl_2  : INTEGER RANGE 0 to 48;  -- temporary position
	 		BEGIN
	 			IF clr='0' THEN 
	 				pdl_1 := 32;
	 				pdl_2 := 32;
					state <= read_low;
	 			ELSIF CLKBAT'EVENT AND CLKBAT = '1' THEN
					
					case state is
						when read_low =>
							read <= '0';
							state <= wait_ready;
						WHEN wait_ready =>
							IF scan_ready = '1' THEN
								read <= '1';
								state <= read_data;
							ELSE
							    state <= wait_ready;
							END IF;
					WHEN read_data =>	
						IF (("00011011" & scan_code) = a)  THEN
	 				 	 	pdl_1 := pdl_1 + 1;
							paddle_1 <= pdl_1;
							state <= read_low;
	 				 		 
						ELSIF (("11110000" & scan_code) = b) then
								pdl_1 :=pdl_1;
								paddle_1 <= pdl_1;
								state <= read_low;
	 				 	ELSIF (("00011101" & scan_code) = c) THEN 
	 						 		pdl_1 := pdl_1 - 1;
								paddle_1 <= pdl_1;
								state <= read_low;
		 				  	 
						elsif (("11110000" & scan_code) = d) then
								pdl_1 :=pdl_1;
								paddle_1 <= pdl_1;
								state <= read_low;
	 			 		
						elsIF (("01001100" & scan_code) = e) THEN
	 						 
	 					    	pdl_2 := pdl_2 + 1;
								paddle_2 <=pdl_2;
								state <= read_low;
	 				 		 
						elsif (("11110000" & scan_code) = f) then
								pdl_2 :=pdl_2;
								state <= read_low;
		 				ELSIF (("01001101" & scan_code) = g) THEN 
	 						 
	 					    	pdl_2 := pdl_2 - 1;
								paddle_2 <=pdl_2;
								state <= read_low;
	 					 	 
						elsif	("11110000" & scan_code = h) then
								pdl_2 :=pdl_2;
								paddle_2 <=pdl_2;
								state <= read_low;
					
						END IF;
				
					end case;
				END IF;
										
												
				IF pdl_1 >16 AND pdl_1 < 48 THEN
					Paddle_1 <=pdl_1;
				ELSIF pdl_1 <= 15 THEN
					Paddle_1 <= 16;
				ELSIF pdl_1 >= 49 THEN
					Paddle_1 <= 48;
				END IF;
				IF pdl_2 >16 AND pdl_2 < 48 THEN
					Paddle_2 <=pdl_2;
				ELSIF pdl_2 <= 15 THEN
					Paddle_2 <= 16;
				ELSIF pdl_2 >= 49 THEN
					Paddle_2 <= 48;
				END IF;

			END PROCESS;
END position;
