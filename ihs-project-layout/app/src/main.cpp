#include <stdio.h>	/* printf */
#include <stdlib.h>	/* malloc, atoi, rand... */
#include <string.h>	/* memcpy, strlen... */
#include <stdint.h>	/* uints types */
#include <sys/types.h>	/* size_t ,ssize_t, off_t... */
#include <unistd.h>	/* close() read() write() */
#include <fcntl.h>	/* open() */
#include <sys/ioctl.h>	/* ioctl() */
#include <errno.h>	/* error codes */
#include <bitset>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include <string>
#include <iostream>

using namespace std;

#include "display.h"
#include "ioctl_cmds.h"

string char_to_stringbit(char n){
	string result;
	switch(n){
		case '0': 
			result = BIN_0;
			break;
		case '1': 
			result = BIN_1;
			break;
		case '2': 
			result = BIN_2;		
			break;
		case '3': 
			result = BIN_3;
			break;
		case '4': 
			result = BIN_4;
			break;
		case '5': 
			result = BIN_5;
			break;
		case '6': 
			result = BIN_6;
			break;
		case '7': 
			result = BIN_7;
			break;
		case '8': 
			result = BIN_8;
			break;
		case '9': 
			result = BIN_9;
			break;
		default:
			result = "";
	}

	return result;
}

void string_to_display(string min_string, string sec_string){
	string bit_string = "";
	bit_string += char_to_stringbit(sec_string[0]);
	bit_string += char_to_stringbit(sec_string[1]);
	bit_string += char_to_stringbit(min_string[0]);
	bit_string += char_to_stringbit(min_string[1]);

	unsigned long int data;

	data = bitset<28>(bit_string).to_ulong();

	//writeLDisplay(fd, data);
	
}


void int_to_string(int number, string &min_string, string &sec_string){
    unsigned int minutes = number / 60;
    unsigned int seconds = number % 60;

    if(seconds < 10){
		sec_string = "0" + to_string(seconds);
	}
	else{
		sec_string = to_string(seconds);
	}
    min_string = "0" + to_string(minutes);

	string_to_display(sec_string, min_string);

}

void writeLDisplay(int fd, int data){
	// ioctl(fd, WR_L_DISPLAY);
	// retval = write(fd, &data, sizeof(data));
	// printk("wrote %d bytes\n", retval);
}

void writeRDisplay(int fd, int data){
	// ioctl(fd, WR_R_DISPLAY);
	// retval = write(fd, &data, sizeof(data));
	// printk("wrote %d bytes\n", retval);
}

void writeGreenLed(int fd, int data){
	// ioctl(fd, WR_GREEN_LEDS);
	// retval = write(fd, &data, sizeof(data));
	// printk("wrote %d bytes\n", retval);
}

void writeRedLed(int fd, int data){
	// ioctl(fd, WR_RED_LEDS);
	// retval = write(fd, &data, sizeof(data));
	// printk("wrote %d bytes\n", retval);
}

int readButton(int fd, int data){
	// ioctl(fd, RD_PBUTTONS);
	// read(fd, &data, 1);
	// printk("new data: 0x%X\n", data);

	return data;
}

int readSwitch(int fd, int data){
	// ioctl(fd, RD_SWITCHES);
	// read(fd, &data, 1);
	// printk("new data: 0x%X\n", data);

	return data;
}

int main(int argc, char** argv){
	//int fd, retval;
	//if(argc < 2){
	//	printf("Syntax: %s <device file path>\n", argv[0]);
	//	return -EINVAL;
	//}

	//if((fd = open(argv[1], O_RDWR)) < 0){
	//	fprintf(stderr, "Error opening file %s\n", argv[1]);
	//	return -EBUSY;
	//}

	// window setup
	sf::VideoMode videomode(800, 600);
    sf::RenderWindow window(videomode, "Project IHS", sf::Style::Titlebar | sf::Style::Default); //close button and not resizable

	window.setFramerateLimit(12);

	//text variables
	sf::Font font;
    font.loadFromFile("../fonts/Hack-Regular.ttf");

    sf::Text text("", font, 30);
    text.setFillColor(sf::Color(14, 107, 14));

	sf::Text timer_text;
    timer_text.setFont(font);
    timer_text.setPosition(200, 200);
    timer_text.setCharacterSize(30);
    timer_text.setFillColor(sf::Color::White);



    sf::Text challenge1_text("", font, 30);
    challenge1_text.setFillColor(sf::Color(14, 107, 14));

	sf::String typedtext = "Ola, vejo que voce encontrou minha caixa...\n\nEspero (ou nao) que voce consiga completar\nos desafios que preparei para hoje.\n\nMas cuidado com o tempo e tente nao perder\na cabeca!!\n\nASS ?";

    sf::String challenge1_string = "Voce deveria saber matematica";

	//time variables
	sf::Clock clock_text;
    sf::Time elapsedtime_text;

    sf::Clock temp_clock;
    sf::Time temp_time;

	sf::Clock clock_bomb;
	sf::Time time_bomb;

	//context variables

	int screen = 1;

	unsigned int data = 0x40404079;
	unsigned int switches = 0x00000;
	unsigned int oldSwitches = 0x00000;
	unsigned int buttons = 0x0;
	unsigned int greenLeds = 0x00000;
	unsigned int redLeds = 0x0;

	unsigned int temp;

	unsigned int seconds;
	string sec_string, min_string;
	
	int currMatrix[18];

	//app loop
	while(window.isOpen()){
		//event controller
		sf::Event event;
		
		while(window.pollEvent(event)){
			if(event.type == sf::Event::Closed){
				window.close();
			}
		}
		window.clear();

		if (screen != 1 && screen != 6){
			//timer interface
			time_bomb = clock_bomb.getElapsedTime();
			seconds = 300 - time_bomb.asSeconds();
			int_to_string(seconds, min_string, sec_string);
			timer_text.setString(min_string + ":" + sec_string);
			window.draw(timer_text);

			if (seconds <= 0){
				screen = 6; //fim de jogo
				text.setString("");
				typedtext.insert(0, "BOOM!!!");

				clock_text.restart();
			}
		}


		switch(screen){
			case 0:
				// Reading from buttons
				//ioctl(fd, RD_PBUTTONS);
				//read(fd, &buttons, 1);

				if(buttons == 0b1110) {
					window.clear(sf::Color::Green);
					screen = 2;

					data = 0b0001;
					//ioctl(fd, WR_RED_LEDS);
					//write(fd, &data, sizeof(data));
				}
				break;

			case 1:
				// text appering
				elapsedtime_text += clock_text.restart();
				while(elapsedtime_text >= sf::seconds(.1f)){
					elapsedtime_text -= sf::seconds(.1f);
					if(typedtext.getSize() > 0){
						text.setString(text.getString() + typedtext[0]);
						typedtext = typedtext.toAnsiString().substr(1);


						temp_clock.restart();
					}
					//when text ends, screen needs to change
					else{ 
						temp_time = temp_clock.getElapsedTime();
						//wait 5 seconds to go to screen 2
						
						if (temp_time >= sf::seconds(5.0f)){
							screen = 2;
							
							text.setString("");
							typedtext.insert(0, "Voce deveria saber matematica");

							clock_text.restart();
							clock_bomb.restart();
						}
					}
				}
				window.draw(text);
				window.display();
				break;

			case 2:
				

				//screen interface
				elapsedtime_text += clock_text.restart();
				while(elapsedtime_text >= sf::seconds(.1f)){
					elapsedtime_text -= sf::seconds(.1f);
					if(typedtext.getSize() > 0){
						text.setString(text.getString() + typedtext[0]);
						typedtext = typedtext.toAnsiString().substr(1);
					}
				}
				window.draw(text);
				window.display();

				// fpga and program logic
				//switches = readSwitch(fd, switches);

				// Answer: 001 010 101 000 010 101 (0x0AA15)
				if(switches == 0x0AA15){
					window.clear();
					screen = 3;

					//writeRedLed(fd, 0b0011);
				}
				break;

			case 4:
			{
				//switches = readSwitch(fd, switches);

				// Ask to reset all switches to 0
				if(switches != 0x00000){
					/*
					TODO: print warning to reset all switches on screen
					*/
					break;
				}

				// Compare old state with current one to create a mask
				unsigned int mask = switches ^ oldSwitches;

				for(int i = 5; i >= 0; i--){
					for(int j = 2; j >= 0; j--){
						temp = (mask % 2);
						mask >> 1;

						// The mask will diffuse itself to surrounding cells, inverting them
						if(temp == 1){
							if(i > 0)
								currMatrix[(i-1)*3 + j] ^= 1;
							if(i < 5)
								currMatrix[(i+1)*3 + j] ^= 1;
							if(j > 0)
								currMatrix[i*3 + (j-1)] ^= 1;
							if(j < 2)
								currMatrix[i*3 + (j+1)] ^= 1;
						}
					}
				}

		        // Save current matrix state for next iteration and pass it to green leds
				greenLeds = 0x00000;
				int k = 1;
				for(int i = 5; i >= 0; i--){
					for(int j = 2; j >= 0; j--){
						greenLeds += currMatrix[i*3 + j] * k;
						k *= 2;
					}
				}
				//writeGreenLed(fd, greenLeds);
				oldSwitches = greenLeds;

				/*
				TODO: pass matrix to screen
				*/


				if(switches == 0x3FFFF){ // All on
					window.clear(sf::Color::Green);
					screen = 5;

					//writeRedLed(fd, 0b1111);
				}
				break;
			}
			case 5:
				// Venceu porra :)

				
				break;

			case 6:
				// Perdeu :(

				//screen interface
				elapsedtime_text += clock_text.restart();
				while(elapsedtime_text >= sf::seconds(.1f)){
					elapsedtime_text -= sf::seconds(.1f);
					if(typedtext.getSize() > 0){
						text.setString(text.getString() + typedtext[0]);
						typedtext = typedtext.toAnsiString().substr(1);
					}
				}
				window.draw(text);
				window.display();
				break;

			default:
				break;
				// Ih, entrou no vazio
		}
	}

	//close(fd);
}

/*
Ao tentar pausar o jogo:
"am I a joke to you?" e não pausa :)
*/