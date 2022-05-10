#include <stdio.h>	   /* printf */
#include <stdlib.h>	   /* malloc, atoi, rand... */
#include <string.h>	   /* memcpy, strlen... */
#include <stdint.h>	   /* uints types */
#include <sys/types.h> /* size_t ,ssize_t, off_t... */
#include <unistd.h>	   /* close() read() write() */
#include <fcntl.h>	   /* open() */
#include <sys/ioctl.h> /* ioctl() */
#include <errno.h>	   /* error codes */
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


#define BUTTON_AMOUNT 6


string char_to_stringbit(char n)
{
	string result;
	switch (n)
	{
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

void writeLDisplay(int fd, unsigned int data)
{
	ioctl(fd, WR_L_DISPLAY);
	write(fd, &data, sizeof(data));
	//printk("wrote %d bytes\n", retval);
}

void writeRDisplay(int fd, unsigned int data)
{
	ioctl(fd, WR_R_DISPLAY);
	write(fd, &data, sizeof(data));
	//printk("wrote %d bytes\n", retval);
}

void writeGreenLed(int fd, unsigned int data)
{
	ioctl(fd, WR_GREEN_LEDS);
	write(fd, &data, sizeof(data));
	// printk("wrote %d bytes\n", retval);
}

void writeRedLed(int fd, unsigned int data)
{
	ioctl(fd, WR_RED_LEDS);
	write(fd, &data, sizeof(data));
	// printk("wrote %d bytes\n", retval);
}

unsigned int readButton(int fd, unsigned int data)
{
	ioctl(fd, RD_PBUTTONS);
	read(fd, &data, 1);
	// printk("new data: 0x%X\n", data);

	return data;
}

unsigned int readSwitch(int fd, unsigned int data)
{
	ioctl(fd, RD_SWITCHES);
	read(fd, &data, 3);
	// printk("new data: 0x%X\n", data);

	return data;
}

void string_to_display(int fd, string min_string, string sec_string)
{
	string bit_string = "";
	bit_string += char_to_stringbit(sec_string[0]);
	bit_string += char_to_stringbit(sec_string[1]);
	bit_string += char_to_stringbit(min_string[0]);
	bit_string += char_to_stringbit(min_string[1]);

	unsigned long int data;

	data = bitset<32>(bit_string).to_ulong();

	writeLDisplay(fd, data);
}

void int_to_string(int fd, int number, string &min_string, string &sec_string)
{
	unsigned int minutes = number / 60;
	unsigned int seconds = number % 60;

	if (seconds < 10)
	{
		sec_string = "0" + to_string(seconds);
	}
	else
	{
		sec_string = to_string(seconds);
	}
	min_string = "0" + to_string(minutes);

	string_to_display(fd, sec_string, min_string);
}

int main(int argc, char **argv)
{
	int fd, retval;
	if(argc < 2){
		printf("Syntax: %s <device file path>\n", argv[0]);
		return -EINVAL;
	}

	if((fd = open(argv[1], O_RDWR)) < 0){
		fprintf(stderr, "Error opening file %s\n", argv[1]);
		return -EBUSY;
	}

	// window setup
	sf::VideoMode videomode(800, 600);
	sf::RenderWindow window(videomode, "Project IHS", sf::Style::Titlebar | sf::Style::Default); //close button and not resizable

	// shapes
	sf::CircleShape square1(30, 4);
	square1.setFillColor(sf::Color::Magenta);
	square1.setPosition(20.f, 50.f);

	sf::CircleShape square2(30, 4);
	square2.setFillColor(sf::Color::Magenta);
	square2.setPosition(100.f, 50.f);

	sf::CircleShape square3(30, 4);
	square3.setFillColor(sf::Color::Blue);
	square3.setPosition(180.f, 50.f);

	sf::CircleShape square4(30, 4);
	square4.setFillColor(sf::Color::Magenta);
	square4.setPosition(20.f, 120.f);

	sf::CircleShape square5(30, 4);
	square5.setFillColor(sf::Color::Blue);
	square5.setPosition(100.f, 120.f);

	sf::CircleShape square6(30, 4);
	square6.setFillColor(sf::Color::Magenta);
	square6.setPosition(180.f, 120.f);

	sf::CircleShape square7(30, 4);
	square7.setFillColor(sf::Color::Blue);
	square7.setPosition(20.f, 190.f);

	sf::CircleShape square8(30, 4);
	square8.setFillColor(sf::Color::Magenta);
	square8.setPosition(100.f, 190.f);

	sf::CircleShape square9(30, 4);
	square9.setFillColor(sf::Color::Blue);
	square9.setPosition(180.f, 190.f);

	sf::CircleShape square10(30, 4);
	square10.setFillColor(sf::Color::Magenta);
	square10.setPosition(20.f, 260.f);

	sf::CircleShape square11(30, 4);
	square11.setFillColor(sf::Color::Magenta);
	square11.setPosition(100.f, 260.f);

	sf::CircleShape square12(30, 4);
	square12.setFillColor(sf::Color::Magenta);
	square12.setPosition(180.f, 260.f);

	sf::CircleShape square13(30, 4);
	square13.setFillColor(sf::Color::Magenta);
	square13.setPosition(20.f, 330.f);

	sf::CircleShape square14(30, 4);
	square14.setFillColor(sf::Color::Blue);
	square14.setPosition(100.f, 330.f);

	sf::CircleShape square15(30, 4);
	square15.setFillColor(sf::Color::Magenta);
	square15.setPosition(180.f, 330.f);

	sf::CircleShape square16(30, 4);
	square16.setFillColor(sf::Color::Blue);
	square16.setPosition(20.f, 400.f);

	sf::CircleShape square17(30, 4);
	square17.setFillColor(sf::Color::Magenta);
	square17.setPosition(100.f, 400.f);

	sf::CircleShape square18(30, 4);
	square18.setFillColor(sf::Color::Blue);
	square18.setPosition(180.f, 400.f);


	window.setFramerateLimit(12);

	//text variables
	sf::Font font;
	font.loadFromFile("/home/de2i-150/Desktop/Projeto-IHS/IHS-Project/ihs-project-layout/app/fonts/Hack-Regular.ttf");

	sf::Text text("", font, 16);
	text.setFillColor(sf::Color(39, 225, 122));

	sf::Text challenge1_text("", font, 16);
	challenge1_text.setFillColor(sf::Color(39, 225, 122));

	sf::Music typing;

	if (!typing.openFromFile("/home/de2i-150/Desktop/Projeto-IHS/IHS-Project/ihs-project-layout/app/assets/sounds/typing.ogg"))
	{
		cout << "Error during audio file import" << endl;
	}

	// to use this music file uncomment the code bellow
	// typing.play();

	sf::String typedtext = "[root@mbois ~] $ Ola, vejo que voce encontrou minha caixa...\n\n[root@mbois ~] $ Espero (ou nao) que voce consiga completar\nos desafios que preparei para hoje.\n\n[root@mbois ~] $ Mas cuidado com o tempo e tente nao perder\na cabeca!!\n\n[root@mbois ~] $ ASS ?";

	sf::String challenge1_string = "[root@mbois ~] $ Voce deveria saber matematica";

	//time variables
	sf::Clock clock_text;
	sf::Time elapsedtime_text;

	sf::Clock temp_clock;
	sf::Time temp_time;

	sf::Clock clock_bomb;
	sf::Time time_bomb;

	//context variables

	int screen = 0;

	unsigned int data = 0x40404079;
	unsigned int switches = 0x00000;
	unsigned int oldSwitches = 0x0AA15;
	unsigned int buttons = 0x0;
	unsigned int greenLeds = 0x0;
	unsigned int redLeds = 0x00000;

	unsigned int temp;
	int i, j;
	int old_button = 0xF;
	int flag = 1;
	int flag_correct = 1;
	int input[BUTTON_AMOUNT];


	unsigned int seconds;
	string sec_string, min_string;

	int currMatrix[18];

	//app loop
	while (window.isOpen())
	{
		//event controller
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}
		window.clear(sf::Color(48, 49, 52));

		if(screen == 0){
			typing.play();
			screen = 1;
		}

		if (screen != 1 && screen != 6)
		{
			//timer interface
			time_bomb = clock_bomb.getElapsedTime();
			seconds = 300 - time_bomb.asSeconds();
			int_to_string(fd, seconds, min_string, sec_string);		

			if (seconds <= 0)
			{
				screen = 6; //fim de jogo
				text.setString("");
				typedtext.insert(0, "BOOM!!!");

				clock_text.restart();
			}
		}

		switch (screen) {
			case 1:
				//reset all
				data = 0x0;
				writeLDisplay(fd, data);
				writeRDisplay(fd, data);
				writeGreenLed(fd, data);
				writeRedLed(fd, data);

				// text appering
				
				elapsedtime_text += clock_text.restart();
				while (elapsedtime_text >= sf::seconds(.001f))
				{
					elapsedtime_text -= sf::seconds(.001f);
					if (typedtext.getSize() > 0)
					{
						text.setString(text.getString() + typedtext[0]);
						typedtext = typedtext.toAnsiString().substr(1);

						temp_clock.restart();
					}
					//when text ends, screen needs to change
					else
					{
						temp_time = temp_clock.getElapsedTime();
						//wait 5 seconds to go to screen 2

						if (temp_time >= sf::seconds(5.0f))
						{
							screen = 2;
							data = 0xFFFFFFFF;
							writeRDisplay(fd, data);

							text.setString("");
							typedtext.insert(0, "[root@mbois ~] $ Voce deveria saber matematica...");

							clock_text.restart();
							clock_bomb.restart();
						}
					}
				}
				window.draw(text);
				break;

			case 2:
				//screen interface
				elapsedtime_text += clock_text.restart();
				while (elapsedtime_text >= sf::seconds(.1f))
				{
					elapsedtime_text -= sf::seconds(.1f);
					if (typedtext.getSize() > 0)
					{
						text.setString(text.getString() + typedtext[0]);
						typedtext = typedtext.toAnsiString().substr(1);
					}
				}
				window.draw(text);

				// fpga and program logic

				data = 0x90F999A4;
				writeRDisplay(fd, data);
				switches = readSwitch(fd, switches);

				if (switches == 0x23B6) // 9142 em hexa
				{
					data = 0x0;
					writeRDisplay(fd, data);
					text.setString("");
					typedtext.insert(0, "Isso faz algum sentido?");
					screen = 3;
					window.clear();
				}
				break;
			
			case 3:
				//screen interface
				elapsedtime_text += clock_text.restart();
				while (elapsedtime_text >= sf::seconds(.1f))
				{
					elapsedtime_text -= sf::seconds(.1f);
					if (typedtext.getSize() > 0)
					{
						text.setString(text.getString() + typedtext[0]);
						typedtext = typedtext.toAnsiString().substr(1);
					}
				}
				window.draw(text);

				// shapes
				window.draw(square1);
				window.draw(square2);
				window.draw(square3);
				window.draw(square4);
				window.draw(square5);
				window.draw(square6);
				window.draw(square7);
				window.draw(square8);
				window.draw(square9);
				window.draw(square10);
				window.draw(square11);
				window.draw(square12);
				window.draw(square13);
				window.draw(square14);
				window.draw(square15);
				window.draw(square16);
				window.draw(square17);
				window.draw(square18);
				
				//fpga and program logic

				switches = readSwitch(fd, switches);

				writeRedLed(fd, switches);

				// Answer: 001 010 101 000 010 101 (0x0AA15)
				if(switches == 0x0AA15){
					text.setString("");
					typedtext.insert(0, "Tem alguma coisa piscando,\nvoce deveria prestar atencao");
					screen = 4;
					temp_clock.restart();
					i = 0;
					flag = 1;
					old_button = 0xF;

					writeGreenLed(fd, 0x0);
				}
				break;
			
			case 5:
			{
				//screen interface
				elapsedtime_text += clock_text.restart();
				while (elapsedtime_text >= sf::seconds(.1f))
				{
					elapsedtime_text -= sf::seconds(.1f);
					if (typedtext.getSize() > 0)
					{
						text.setString(text.getString() + typedtext[0]);
						typedtext = typedtext.toAnsiString().substr(1);
					}
				}
				window.draw(text);

				switches = readSwitch(fd, switches);

				// Compare old state with current one to create a mask
				unsigned int mask = switches ^ oldSwitches;
				//printf("mask: %p\n", mask);

				if(mask != 0){
					for (int i = 5; i >= 0; i--)
					{
						for (int j = 2; j >= 0; j--)
						{
							temp = (mask % 2);
							mask = mask >> 1;

							// The mask will diffuse itself to surrounding cells, inverting them
							if (temp == 1)
							{
								currMatrix[i*3 + j] ^= 1;
								if (i > 0) // up
									currMatrix[(i - 1) * 3 + j] ^= 1;
								if (i < 5) // down
									currMatrix[(i + 1) * 3 + j] ^= 1;
								if (j > 0) // left
									currMatrix[i * 3 + (j - 1)] ^= 1;
								if (j < 2) // right
									currMatrix[i * 3 + (j + 1)] ^= 1;
							}
						}
					}

					// Save current matrix state for next iteration and pass it to red leds
					redLeds = 0x00000;
					int pot = 0;
					
					for (int i = 5; i >= 0; i--)
					{
						for (int j = 2; j >= 0; j--)
						{
							pot = 17 - (i * 3 + j); // The number to elevate 2 by
							for(int m = 1; m <= pot; ++m)
								redLeds += currMatrix[i * 3 + j] * m;

						}
					}

					//printf("redLeds: %p\n", redLeds);
					writeRedLed(fd, redLeds);
					
					oldSwitches = redLeds;
				}


				/*
					TODO: pass matrix to screen
					*/

				if (switches == 0x3FFFF)
				{ // All on
					window.clear(sf::Color::Green);
					screen = 5;

					// writeGreenLed(fd, 0b1111);
				}
				break;
			}
			case 4:{
				//screen interface
				elapsedtime_text += clock_text.restart();
				while (elapsedtime_text >= sf::seconds(.1f))
				{
					elapsedtime_text -= sf::seconds(.1f);
					if (typedtext.getSize() > 0)
					{
						text.setString(text.getString() + typedtext[0]);
						typedtext = typedtext.toAnsiString().substr(1);
					}
				}
				window.draw(text);

				// Genius
				
				int seqGreenLeds[BUTTON_AMOUNT] = {48, 12, 192, 48, 3, 48}; // 2^4+2^5, 2^2+2^3, 2^6+2^7, 2^0+2^1
				

				temp_time = temp_clock.getElapsedTime();
				if(temp_time >= sf::seconds(3.0f) && i < BUTTON_AMOUNT && flag){
					temp_clock.restart();
					writeGreenLed(fd, seqGreenLeds[i]);
					i++;
				}
				else if (i >= BUTTON_AMOUNT && flag && temp_time >= sf::seconds(3.0f)){
					flag = 0;
					writeGreenLed(fd, 0x0);
					i = 0;
				}

				int answer[BUTTON_AMOUNT] = {11, 13, 7, 11, 14, 11}; // 0xB, 0xD, 0x7, 0xE
				int button = readButton(fd, button);
				

				if(button != 0xF && button != old_button && j < BUTTON_AMOUNT){
					old_button = button;
					input[j] = button;
					
					j++;
				}


				if (j >= BUTTON_AMOUNT){
					printf("%d %d %d %d %d %d\n", input[0], input[1], input[2], input[3], input[4], input[5]);
					//char *str;
					//sprintf(str, "%d %d %d %d", input[3], input[2], input[1], input[0]);
					//text.setString(str);
					//window.draw(text);
					//window.display();
					for (int k = 0; k < BUTTON_AMOUNT; k++){
						if(answer[k] != input[k]){
							printf("\n%d %d", answer[k], input[k]);
							screen = 6;
							printf("errou\n");
							text.setString("");
							typedtext.insert(0, "BOOM!!!");

							flag_correct = 0;

							clock_text.restart();
							break;
						}
					}
					if(flag_correct){
						screen = 5;
						text.setString("");
						typedtext.insert(0, "Quinta fase");
						clock_text.restart();
					}
				}
				


				break;
			}
			case 6:


				// Perdeu :(

				//screen interface
				elapsedtime_text += clock_text.restart();
				while (elapsedtime_text >= sf::seconds(.1f))
				{
					elapsedtime_text -= sf::seconds(.1f);
					if (typedtext.getSize() > 0)
					{
						text.setString(text.getString() + typedtext[0]);
						typedtext = typedtext.toAnsiString().substr(1);
					}
				}
				window.draw(text);
				break;

			default:
				break;
				// Ih, entrou no vazio
		}

		window.display();
	}

	

	//close(fd);
}

/*
Ao tentar pausar o jogo:
"am I a joke to you?" e não pausa :)
//Adorei - Vivi
*/