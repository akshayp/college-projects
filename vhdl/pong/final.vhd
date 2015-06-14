LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.STD_LOGIC_UNSIGNED.ALL;
USE IEEE.STD_LOGIC_ARITH.ALL;

ENTITY FINAL IS
        PORT(Clock,CLK,CLKBAT,CLR    : IN STD_LOGIC;
        	 scan_ready		 		 : IN STD_LOGIC;
        	 SERVE1, SERVE2          : IN STD_LOGIC;
			 scan_code 				 : IN STD_LOGIC_VECTOR(7 DOWNTO 0);
			 Hexout					 : OUT STD_LOGIC_VECTOR(127 DOWNTO 0);
			 read         			 : OUT STD_LOGIC;
        	 Horiz_sync, Vert_sync   : OUT STD_LOGIC;
             RED, GREEN, BLUE        : OUT STD_LOGIC);
END FINAL;

ARCHITECTURE NEW1 OF FINAL IS

	SIGNAL PADDLE1, PADDLE2 	: INTEGER RANGE 0 TO 48;
	SIGNAL POSITION1, POSITION2 : INTEGER RANGE 0 TO 48;
	SIGNAL SCORE11, SCORE22 	: INTEGER RANGE 0 TO 9;
	SIGNAL ROW, COLUMN			: INTEGER RANGE 0 TO 64;
	SIGNAL OUT1,OUT2     		: STD_LOGIC_VECTOR(6 DOWNTO 0);
	
	COMPONENT bat 
		PORT ( clr, CLKbat     			: IN STD_LOGIC;
				scan_code				: IN STD_LOGIC_VECTOR(7 DOWNTO 0);
				scan_ready			 	: IN std_logic; 
		 		read 					: OUT std_logic;
		  		paddle_1, paddle_2 		: OUT INTEGER	RANGE 0 to 48);
		  					
		  
	END COMPONENT;
	
	COMPONENT random
		PORT ( clk,clr	 			  : IN STD_LOGIC;
		  	   POSITION1, POSITION2   : OUT INTEGER RANGE 0 TO 48);
		END COMPONENT;
	
	COMPONENT PING
       PORT( CLK,CLR              : IN STD_LOGIC;
             SERVE1, SERVE2       : IN STD_LOGIC;
			 POSITION1, PADDLE1	  : IN INTEGER RANGE 0 TO 48;
             POSITION2, PADDLE2	  : IN INTEGER RANGE 0 TO 48;
             SCORE11,SCORE22      : INOUT INTEGER RANGE 0 TO 9; 
             ROW, COLUMN		  : OUT INTEGER RANGE 0 TO 64;                                                    
              OUT1,OUT2           : OUT STD_LOGIC_VECTOR(6 DOWNTO 0); 
               HEXOUT              : OUT STD_LOGIC_VECTOR(127 DOWNTO 0)); 
    END COMPONENT;
    
    COMPONENT display 
        PORT(Clock	                    : IN STD_LOGIC;
        	  ROW, COLUMN				: IN INTEGER RANGE 0 TO 64;
        	  PADDLE1,PADDLE2			: IN INTEGER RANGE 0 TO 48;
        	  OUT1,OUT2           		: IN STD_LOGIC_VECTOR(6 DOWNTO 0);
              Horiz_sync, Vert_sync     : OUT STD_LOGIC;
              RED, GREEN, BLUE          : OUT STD_LOGIC);
        END COMPONENT;
	BEGIN
	U1: bat
		PORT MAP(clr,CLKBAT, scan_code, scan_ready, read, PADDLE1,PADDLE2);
	U2: random
		PORT MAP(clk,clr,POSITION1,POSITION2);
	U3: PING
		PORT MAP(CLK,CLR,SERVE1,SERVE2, POSITION1,PADDLE1,POSITION2,PADDLE2,SCORE11,SCORE22,ROW,COLUMN,OUT1,OUT2,HEXOUT);
	U4: display
		PORT MAP(Clock,ROW,COLUMN,PADDLE1,PADDLE2,OUT1,OUT2,Horiz_sync,Vert_sync,RED,GREEN,BLUE);
	END NEW1;
		