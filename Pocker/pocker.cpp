#include <SFML/Graphics.hpp>
#include <stdio.h> 
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <conio.h>
#include <sstream>
#include <math.h>

#pragma warning(disable : 4996)
using namespace sf;

Texture cardstexture, fieldtexture;
Sprite cards, cardh1, cardh2, cardc1, cardc2, card1, card2, card3, card4, card5, field;
std::ostringstream print, print_rate;
Font font;
Text bank("", font, 40), bank_hum("", font, 40), bank_com("", font, 40), rate_cchip("", font, 40), rate_hchip("", font, 40), hrate("", font, 40), winner("", font, 40), loser("", font, 40);
Texture passtexture, ratetexture, calltexture, checktexture, oktexture, extexture, conttexture;
Sprite pass, rate, call, check, cpass, crate, ccall, ccheck, ok, ex, cont;
Texture dealertexture, chiptexture;
Sprite dealer, cchip, hchip;
Event click;

char end_game = 0, lvl = 3, chance;
char card_c[2], card_h[2];
char flop[3];
char turn, turn_flag = 0, river, river_flag = 0;
char dealer_chip, bluff;
unsigned int bank_c = 1000, bank_h = 1000, bank_all;
char pair_table[13], set_table[13], straight[13], real_straight[13], Dflash[13], Cflash[13], Hflash[13], Sflash[13], DFflag = 0, CFflag = 0, HFflag = 0, SFflag = 0;
char TCombination[9], HCCombination[10], HHCombination[10];
float flop_prob[11][2] =
{
	{ 5.64, 19.74 }, { 5.31, 16.99 }, { 4.98, 14.22 }, { 4.66, 11.45 }, { 4.80, 10.45 },
	{ 4.47, 8.08 }, { 4.14, 5.22 }, { 3.82, 2.61 }, { 4.33, 10.94 }, { 3.49, 1.12 }, { 3.49, 0 }
};
float turn_river_prob[15][2] =
{
	{ 2.13, 2.17 }, { 4.26, 4.35 }, { 6.38, 6.52 },
	{ 8.51, 8.70 }, { 10.64, 10.87 }, { 12.77, 13.04 },
	{ 14.89, 15.22 }, { 17.02, 17.39 }, { 19.15, 19.57 },
	{ 21.23, 21.47 }, { 23.40, 23.91 }, { 25.53, 26.09 },
	{ 27.66, 28.26 }, { 29.79, 30.43 }, { 31.91, 32.61 }
};
char hand_card = -1, hand_card_pair = 0, card_premium = 0;
char outs_straight, outST[13] = { 0 }, outs_flush;
char real_straightDRO[13], DFDflag = 0, CFDflag = 0, HFDflag = 0, SFDflag = 0;

void menu(RenderWindow& window) {
	Texture menutexture1, menutexture2, menubackground, lvl1texture, lvl2texture, lvl3texture;
	menutexture1.loadFromFile("images/111.png");
	menutexture2.loadFromFile("images/333.png");
	menubackground.loadFromFile("images/menuback.png");
	lvl1texture.loadFromFile("images/1.png");
	lvl2texture.loadFromFile("images/2.png");
	lvl3texture.loadFromFile("images/3.png");
	Sprite menu1(menutexture1), menu2(menutexture2), menubg(menubackground), lvl1(lvl1texture), lvl2(lvl2texture), lvl3(lvl3texture);
	bool ismenu = 1;
	int menunum = 0;
	menu1.setPosition(130, 30);
	menu2.setPosition(130, 90);
	lvl1.setPosition(130, 30);
	lvl2.setPosition(130, 90);
	lvl3.setPosition(130, 150);

	while (ismenu) {
		window.clear();
		menu1.setColor(Color::White);
		menu2.setColor(Color::White);
		menunum = 0;
		if (IntRect(130, 30, 300, 50).contains(Mouse::getPosition(window))) { menu1.setColor(Color::Blue); menunum = 1; }
		if (IntRect(130, 90, 300, 50).contains(Mouse::getPosition(window))) { menu2.setColor(Color::Blue); menunum = 2; }
		while (window.pollEvent(click)) if (click.type == click.MouseButtonReleased && click.mouseButton.button == Mouse::Left) {
			if (menunum == 1) { ismenu = false; }
			if (menunum == 2) { window.close(); exit(1); }

		}
		window.draw(menubg);
		window.draw(menu1);
		window.draw(menu2);
		window.display();
	}
	ismenu = 1;
	while (ismenu) {
		window.clear();
		lvl1.setColor(Color::White);
		lvl2.setColor(Color::White);
		lvl3.setColor(Color::White);
		menunum = 0;
		if (IntRect(130, 30, 150, 50).contains(Mouse::getPosition(window))) { lvl1.setColor(Color::Blue); menunum = 1; }
		if (IntRect(130, 90, 150, 50).contains(Mouse::getPosition(window))) { lvl2.setColor(Color::Blue); menunum = 2; }
		if (IntRect(130, 150, 150, 50).contains(Mouse::getPosition(window))) { lvl3.setColor(Color::Blue); menunum = 3; }
		while (window.pollEvent(click)) if (click.type == click.MouseButtonReleased && click.mouseButton.button == Mouse::Left) {
			if (menunum == 1) { lvl = 1; ismenu = false; }
			if (menunum == 2) { lvl = 2; ismenu = false; }
			if (menunum == 3) { lvl = 3; ismenu = false; }
		}
		window.draw(menubg);
		window.draw(lvl1);
		window.draw(lvl2);
		window.draw(lvl3);
		window.display();
	}
}

void BEGIN() {
	bank_all = 0;
	memset(card_c, 0, 2);
	memset(card_h, 0, 2);
	memset(flop, 0, 3);
	turn = 0; turn_flag = 0;
	river = 0; river_flag = 0;
	hand_card = -1; hand_card_pair = 0; card_premium = 0;
	end_game = 0;
}

void CHECK_STARTHAND() {

	if (card_c[1] % 13 == 0) {
		if (card_c[0] % 13 == 1 || card_c[0] % 13 == 12) hand_card = 0;
		if (card_c[0] % 13 == 2 || card_c[0] % 13 == 11) hand_card = 1;
		if (card_c[0] % 13 == 3 || card_c[0] % 13 == 10) hand_card = 2;
		if (card_c[0] % 13 == 4 || card_c[0] % 13 == 9) hand_card = 3;
	}
	else if (card_c[1] % 13 - card_c[0] % 13 < 5) hand_card = (card_c[1] % 13 - card_c[0] % 13) - 1;

	if (hand_card >= 0 && hand_card <= 3) {
		if (card_c[0] < 14) if (card_c[1] > 13) hand_card += 4;
		if (card_c[0] > 13 && card_c[0] < 27) if (card_c[1] < 14 || card_c[1] > 26) hand_card += 4;
		if (card_c[0] > 26 && card_c[0] < 40) if (card_c[1] < 27 || card_c[1] > 39) hand_card += 4;
		if (card_c[0] > 39 && card_c[0] < 53) if (card_c[1] < 40) hand_card += 4;
	}

	if (hand_card == -1) {
		if (card_c[0] % 13 == card_c[1] % 13) {
			hand_card_pair = 1;
			if (card_c[0] % 13 == 0) hand_card = 9;
			else hand_card = 10;
		}
		else {
			if (card_c[0] < 14) if (card_c[1] > 13) hand_card = 10;
			if (card_c[0] > 13 && card_c[0] < 27) if (card_c[1] < 14 || card_c[1] > 26) hand_card = 10;
			if (card_c[0] > 26 && card_c[0] < 40) if (card_c[1] < 27 || card_c[1] > 39) hand_card = 10;
			if (card_c[0] > 39 && card_c[0] < 53) if (card_c[1] < 40) hand_card = 10;
		}
		if (hand_card == -1) hand_card = 8;
	}

	if (hand_card_pair == 1) if (card_c[0] % 13 == 0 || card_c[0] % 13 > 8) card_premium = 1;
	if (hand_card_pair == 0) {
		if (card_c[1] % 13 == 0) if (card_c[0] % 13 == 12 || card_c[0] % 13 == 11 || card_c[0] % 13 == 10) card_premium = 1;
		if (card_c[1] % 13 == 12) if (card_c[0] % 13 == 11) card_premium = 1;
	}

}

void check_cards(char card, short* x, short* y) {

	char crd = card % 13;
	switch (crd)
	{
	case 0: {
		*x = 0;
		break;
	}
	case 1: {
		*x = 148;
		break;
	}
	case 2: {
		*x = 295;
		break;
	}
	case 3: {
		*x = 443;
		break;
	}
	case 4: {
		*x = 591;
		break;
	}
	case 5: {
		*x = 738;
		break;
	}
	case 6: {
		*x = 885;
		break;
	}
	case 7: {
		*x = 1033;
		break;
	}
	case 8: {
		*x = 1181;
		break;
	}
	case 9: {
		*x = 1329;
		break;
	}
	case 10: {
		*x = 1476;
		break;
	}
	case 11: {
		*x = 1624;
		break;
	}
	case 12: {
		*x = 1771;
		break;
	}
	}

	if (card < 14) *y = 0;
	if (card > 13 && card < 27) *y = 214;
	if (card > 26 && card < 40) *y = 429;
	if (card > 39 && card < 53) *y = 643;

}

void update(RenderWindow& window, char flag) {
	window.clear();
	window.draw(field);
	window.draw(cards);
	print.str("");
	print << bank_all;
	bank.setString("BANK: " + print.str());
	window.draw(bank);
	print.str("");
	print << bank_h;
	bank_hum.setString("Bank: " + print.str());
	window.draw(bank_hum);
	print.str("");
	print << bank_c;
	bank_com.setString("Bank: " + print.str());
	window.draw(bank_com);
	window.draw(cardh1);
	window.draw(cardh2);
	window.draw(cardc1);
	window.draw(cardc2);
	if (flag >= 2) {
		window.draw(card1);
		window.draw(card2);
		window.draw(card3);
	}
	if (flag >= 3) window.draw(card4);
	if (flag >= 4) window.draw(card5);

	window.draw(cchip);
	window.draw(hchip);
	window.draw(dealer);
}

unsigned int COMP_RATE(unsigned int rate) {
	if (bank_c > rate) {
		bank_c -= rate;
		bank_all += rate;
	}
	else {
		bank_all += bank_c;
		rate = bank_c;
		bank_c = 0;
	}
	return rate;
}

void II(RenderWindow& window, char flag, unsigned int rate_h, unsigned int* rate_c, char* check, float prob, char bluff, char* choice) {
	char percent = rand() % 101, bluff_flag = 0;
	unsigned int rise;
	char more_flush = 0, flush_c = 0, flush_h = 0, straight_c = 0, straight_h = 0;

	if (bluff > 74 && lvl == 3) bluff_flag = 1;

	if (flag == 1) {
		if (bluff_flag == 1 || prob > 16) {
			if ((lvl == 3 && percent > 9) || (lvl == 2 && percent > 24) || (lvl == 1 && percent > 49)) {
				if (rate_h == 0) {
					if (card_premium == 1) {
						*rate_c += COMP_RATE((bank_all >> 1) + rand() % (bank_all >> 1 + 1));
						update(window, flag);
						window.draw(crate);
						*choice = 1;
						window.display();
					}
					if (card_premium == 0) {
						*rate_c += COMP_RATE(((bank_all * 3) / 10) + rand() % ((bank_all << 2) / 10 + 1));
						update(window, flag);
						window.draw(crate);
						*choice = 1;
						window.display();
					}
				}
				else {
					if (bank_c < (rate_h - *rate_c)) {
						bank_h += (rate_h - *rate_c) - bank_c;
						bank_all -= (rate_h - *rate_c) - bank_c;
						*rate_c += COMP_RATE(bank_c);
						update(window, flag);
						window.draw(ccall);
						*choice = 3;
						window.display();
					}
					else {
						if (card_premium == 1) {
							if (bank_h == 0) rise = 0; //
							else rise = (rate_h - *rate_c) >> 1 + rand() % ((rate_h - *rate_c) >> 1 + 1);
							*rate_c += COMP_RATE((rate_h - *rate_c) + rise);
							if (rise == 0) {
								update(window, flag);
								window.draw(ccall);
								*choice = 3;
								window.display();
							}
							else {
								update(window, flag);
								window.draw(crate);
								*choice = 1;
								window.display();
							}
						}
						if (card_premium == 0) {
							if (bank_h == 0) rise = 0; //
							else rise = (rate_h - *rate_c) >> 2 + rand() % (3 * (rate_h - *rate_c) >> 2 + 1);
							*rate_c += COMP_RATE((rate_h - *rate_c) + rise);
							if (rise == 0) {
								update(window, flag);
								window.draw(ccall);
								*choice = 3;
								window.display();
							}
							else {
								update(window, flag);
								window.draw(crate);
								*choice = 1;
								window.display();
							}
						}
					}
				}
			}
			else {
				if (rate_h == 0) {
					update(window, flag);
					window.draw(ccheck);
					*choice = 2;
					window.display();
					*check = 1;
				}
				else {
					update(window, flag);
					window.draw(ccall);
					*choice = 3;
					window.display();
					if (bank_c < (rate_h - *rate_c)) {
						bank_h += (rate_h - *rate_c) - bank_c;
						bank_all -= (rate_h - *rate_c) - bank_c;
						*rate_c += COMP_RATE(bank_c);
					}
					else *rate_c += COMP_RATE(rate_h - *rate_c);
				}
			}
		}
		if ((prob > 10 && prob < 16) && bluff_flag == 0) {
			if ((lvl == 3 && percent > 24) || (lvl == 2 && percent > 49) || (lvl == 1 && percent > 74)) {
				if (rate_h == 0) {
					if (hand_card_pair == 0 && card_premium == 0) {
						*rate_c += COMP_RATE((bank_all / 10) + rand() % (bank_all / 5 + 1));
						update(window, flag);
						window.draw(crate);
						*choice = 1;
						window.display();
					}
					else if (card_premium == 1 && hand_card_pair == 0) {
						*rate_c += COMP_RATE(((bank_all * 3) / 10) + rand() % ((bank_all << 2) / 10 + 1));
						update(window, flag);
						window.draw(crate);
						*choice = 1;
						window.display();
					}
					else if (card_premium == 1 && hand_card_pair == 1) {
						*rate_c += COMP_RATE((bank_all >> 1) + rand() % (bank_all >> 1 + 1));
						update(window, flag);
						window.draw(crate);
						*choice = 1;
						window.display();
					}
				}
				else {
					if (bank_c < (rate_h - *rate_c)) {
						bank_h += (rate_h - *rate_c) - bank_c;
						bank_all -= (rate_h - *rate_c) - bank_c;
						*rate_c += COMP_RATE(bank_c);
						window.draw(ccall);
						*choice = 3;
						window.display();
					}
					else {
						if (hand_card_pair == 0 && card_premium == 0) {
							if (bank_h == 0) rise = 0; //
							else rise = (rate_h - *rate_c) / 5 + rand() % (((rate_h - *rate_c) << 2) / 5 + 1);
							*rate_c += COMP_RATE((rate_h - *rate_c) + rise);
							if (rise == 0) {
								update(window, flag);
								window.draw(ccall);
								*choice = 3;
								window.display();
							}
							else {
								update(window, flag);
								window.draw(crate);
								*choice = 1;
								window.display();
							}
						}
						else if (card_premium == 1 && hand_card_pair == 0) {
							if (bank_h == 0) rise = 0; //
							else rise = (rate_h - *rate_c) >> 2 + rand() % (3 * (rate_h - *rate_c) >> 2 + 1);
							*rate_c += COMP_RATE((rate_h - *rate_c) + rise);
							if (rise == 0) {
								update(window, flag);
								window.draw(ccall);
								*choice = 3;
								window.display();
							}
							else {
								update(window, flag);
								window.draw(crate);
								*choice = 1;
								window.display();
							}
						}
						else if (card_premium == 1 && hand_card_pair == 1) {
							if (bank_h == 0) rise = 0; //
							else rise = (rate_h - *rate_c) >> 1 + rand() % ((rate_h - *rate_c) >> 1 + 1);
							*rate_c += COMP_RATE((rate_h - *rate_c) + rise);
							if (rise == 0) {
								update(window, flag);
								window.draw(ccall);
								*choice = 3;
								window.display();
							}
							else {
								update(window, flag);
								window.draw(crate);
								*choice = 1;
								window.display();
							}
						}
					}
				}
			}
			else {
				if (rate_h == 0) {
					update(window, flag);
					window.draw(ccheck);
					*choice = 2;
					window.display();
					*check = 1;
				}
				else {
					update(window, flag);
					window.draw(ccall);
					*choice = 3;
					window.display();
					if (bank_c < (rate_h - *rate_c)) {
						bank_h += (rate_h - *rate_c) - bank_c;
						bank_all -= (rate_h - *rate_c) - bank_c;
						*rate_c += COMP_RATE(bank_c);
					}
					else *rate_c += COMP_RATE(rate_h - *rate_c);
				}
			}
		}
		if (prob < 10 && bluff_flag == 0) {
			if ((lvl == 3 && percent > 74) || (lvl == 2 && percent > 84) || (lvl == 1 && percent > 94)) {
				if (rate_h == 0) {
					if (hand_card_pair == 0 && card_premium == 0) {
						update(window, flag);
						window.draw(cpass);
						*choice = 4;
						window.display();
						end_game = 2;
					}
					else if (card_premium == 1 && hand_card_pair == 0) {
						*rate_c += COMP_RATE((bank_all / 10) + rand() % (bank_all / 5 + 1));
						update(window, flag);
						window.draw(crate);
						*choice = 1;
						window.display();
					}
					else if (card_premium == 1 && hand_card_pair == 1) {
						*rate_c += COMP_RATE(((bank_all * 3) / 10) + rand() % ((bank_all << 2) / 10 + 1));
						update(window, flag);
						window.draw(crate);
						*choice = 1;
						window.display();
					}
				}
				else {
					if (bank_c < (rate_h - *rate_c)) {
						if (hand_card_pair == 0 && card_premium == 0) {
							update(window, flag);
							window.draw(cpass);
							*choice = 4;
							window.display();
							end_game = 2;
						}
						else {
							bank_h += (rate_h - *rate_c) - bank_c;
							bank_all -= (rate_h - *rate_c) - bank_c;
							*rate_c += COMP_RATE(bank_c);
							update(window, flag);
							window.draw(ccall);
							*choice = 3;
							window.display();
						}
					}
					else {
						if (card_premium == 1 && hand_card_pair == 1) {
							if (bank_h == 0) rise = 0; //
							else rise = (rate_h - *rate_c) >> 2 + rand() % (3 * (rate_h - *rate_c) >> 2 + 1);
							*rate_c += COMP_RATE((rate_h - *rate_c) + rise);
							if (rise == 0) {
								update(window, flag);
								window.draw(ccall);
								*choice = 3;
								window.display();
							}
							else {
								update(window, flag);
								window.draw(crate);
								*choice = 1;
								window.display();
							}
						}
						if (card_premium == 1 && hand_card_pair == 0) {
							if (bank_h == 0) rise = 0; //
							else rise = (rate_h - *rate_c) / 6 + rand() % (5 * (rate_h - *rate_c) / 6 + 1);
							*rate_c += COMP_RATE((rate_h - *rate_c) + rise);
							if (rise == 0) {
								update(window, flag);
								window.draw(ccall);
								*choice = 3;
								window.display();
							}
							else {
								update(window, flag);
								window.draw(crate);
								*choice = 1;
								window.display();
							}
						}
						if (hand_card_pair == 0 && card_premium == 0) {
							update(window, flag);
							window.draw(cpass);
							*choice = 4;
							window.display();
							end_game = 2;
						}
					}
				}
			}
			else {
				if (rate_h == 0) {
					update(window, flag);
					window.draw(ccheck);
					*choice = 2;
					window.display();
					*check = 1;
				}
				else {
					update(window, flag);
					window.draw(ccall);
					*choice = 3;
					window.display();
					if (bank_c < (rate_h - *rate_c)) {
						bank_h += (rate_h - *rate_c) - bank_c;
						bank_all -= (rate_h - *rate_c) - bank_c;
						*rate_c += COMP_RATE(bank_c);
					}
					*rate_c += COMP_RATE(rate_h - *rate_c);
				}
			}
		}
	}

	if (flag >= 2) {
		for (char i = 8; i > 4; i--) {
			if (HCCombination[i] != -1) {
				more_flush = 1;
				break;
			}
		}
		if (HCCombination[4] != -1 || DFDflag == 1 || CFDflag == 1 || HFDflag == 1 || SFDflag == 1) flush_c = 1;
		if (DFflag == 1 || CFflag == 1 || HFflag == 1 || SFflag == 1) flush_h = 1;
		for (char i = 0; i < 9; i++)
			if (real_straightDRO[i] == 1 || HCCombination[3] == 1) {
				straight_c = 1;
				break;
			}
		for (char i = 0; i < 9; i++)
			if (real_straight[i] == 1) {
				straight_h = 1;
				break;
			}
		if (bluff_flag == 1 ||
			more_flush == 1 ||
			(flush_c == 1 && flush_h == 0) ||
			(flush_c == 1 && flush_h == 1 && (outs_flush > 9 || HCCombination[4] > 8 || HCCombination[4] == 0)) ||
			(straight_c == 1 && straight_h == 0 && flush_h == 0) ||
			(straight_c == 1 && straight_h == 1 && flush_h == 0 && outs_straight > 5) ||
			(((card_premium == 1 && hand_card_pair == 1) || HCCombination[1] != -1 || HCCombination[2] != -1)) && straight_c == 0 && flush_c == 0) {
			if ((lvl == 3 && percent > 9) || (lvl == 2 && percent > 24) || (lvl == 1 && percent > 49)) {
				if (rate_h == 0) {
					*rate_c += COMP_RATE((bank_all >> 1) + rand() % (bank_all >> 1 + 1));
					update(window, flag);
					window.draw(crate);
					*choice = 1;
					window.display();
				}
				else {
					if (bank_c < (rate_h - *rate_c)) {
						bank_h += (rate_h - *rate_c) - bank_c;
						bank_all -= (rate_h - *rate_c) - bank_c;
						*rate_c += COMP_RATE(bank_c);
						update(window, flag);
						window.draw(ccall);
						*choice = 3;
						window.display();
					}
					else {
						if (bank_h == 0) rise = 0; //
						else rise = (rate_h - *rate_c) >> 1 + rand() % ((rate_h - *rate_c) >> 1 + 1);
						*rate_c += COMP_RATE((rate_h - *rate_c) + rise);
						if (rise == 0) {
							update(window, flag);
							window.draw(ccall);
							*choice = 3;
							window.display();
						}
						else {
							update(window, flag);
							window.draw(crate);
							*choice = 1;
							window.display();
						}
					}
				}
			}
			else {
				if (rate_h == 0) {
					update(window, flag);
					window.draw(ccheck);
					*choice = 2;
					window.display();
					*check = 1;
				}
				else {
					update(window, flag);
					window.draw(ccall);
					*choice = 3;
					window.display();
					if (bank_c < (rate_h - *rate_c)) {
						bank_h += (rate_h - *rate_c) - bank_c;
						bank_all -= (rate_h - *rate_c) - bank_c;
						*rate_c += COMP_RATE(bank_c);
					}
					else *rate_c += COMP_RATE(rate_h - *rate_c);
				}
			}
		}
		else {
			if (HCCombination[1] != -1 || HCCombination[2] != -1 || (card_premium == 1 && hand_card_pair == 1) || straight_c == 1 || flush_c == 1) {
				if ((lvl == 3 && percent > 24) || (lvl == 2 && percent > 49) || (lvl == 1 && percent > 74)) {
					if (rate_h == 0) {
						*rate_c += COMP_RATE(((bank_all * 3) / 10) + rand() % ((bank_all << 2) / 10 + 1));
						update(window, flag);
						window.draw(crate);
						*choice = 1;
						window.display();
					}
					else {
						if (bank_c < (rate_h - *rate_c)) {
							bank_h += (rate_h - *rate_c) - bank_c;
							bank_all -= (rate_h - *rate_c) - bank_c;
							*rate_c += COMP_RATE(bank_c);
							update(window, flag);
							window.draw(ccall);
							*choice = 3;
							window.display();
						}
						else {
							if (bank_h == 0) rise = 0; //
							else rise = (rate_h - *rate_c) >> 1 + rand() % ((rate_h - *rate_c) >> 1 + 1);
							*rate_c += COMP_RATE((rate_h - *rate_c) + rise);
							if (rise == 0) {
								update(window, flag);
								window.draw(ccall);
								*choice = 3;
								window.display();
							}
							else {
								update(window, flag);
								window.draw(crate);
								*choice = 1;
								window.display();
							}
						}
					}
				}
				else {
					if (rate_h == 0) {
						update(window, flag);
						window.draw(ccheck);
						*choice = 2;
						window.display();
						*check = 1;
					}
					else {
						update(window, flag);
						window.draw(ccall);
						*choice = 3;
						window.display();
						if (bank_c < (rate_h - *rate_c)) {
							bank_h += (rate_h - *rate_c) - bank_c;
							bank_all -= (rate_h - *rate_c) - bank_c;
							*rate_c += COMP_RATE(bank_c);
						}
						else *rate_c += COMP_RATE(rate_h - *rate_c);
					}
				}
			}
			else {
				if ((lvl == 3 && percent > 74) || (lvl == 2 && percent > 84) || (lvl == 1 && percent > 94)) {
					if (rate_h == 0) {
						if (river_flag == 1) {
							*rate_c += COMP_RATE((bank_all / 10) + rand() % (bank_all / 5 + 1));
							update(window, flag);
							window.draw(crate);
							*choice = 1;
							window.display();
						}
						if (river_flag == 0) {
							update(window, flag);
							window.draw(cpass);
							*choice = 4;
							window.display();
							end_game = 2;
						}
					}
					else {
						if (bank_c < (rate_h - *rate_c)) {
							if (river_flag == 0) {
								update(window, flag);
								window.draw(cpass);
								*choice = 4;
								window.display();
								end_game = 2;
							}
							else {
								bank_h += (rate_h - *rate_c) - bank_c;
								bank_all -= (rate_h - *rate_c) - bank_c;
								*rate_c += COMP_RATE(bank_c);
								update(window, flag);
								window.draw(ccall);
								*choice = 3;
								window.display();
							}
						}
						else {
							if (river_flag == 1) {
								if (bank_h == 0) rise = 0; //
								else rise = (rate_h - *rate_c) >> 2 + rand() % (3 * (rate_h - *rate_c) >> 2 + 1);
								*rate_c += COMP_RATE((rate_h - *rate_c) + rise);
								if (rise == 0) {
									update(window, flag);
									window.draw(ccall);
									*choice = 3;
									window.display();
								}
								else {
									update(window, flag);
									window.draw(crate);
									*choice = 1;
									window.display();
								}
							}
							if (river_flag == 0) {
								update(window, flag);
								window.draw(cpass);
								*choice = 4;
								window.display();
								end_game = 2;
							}
						}
					}
				}
				else {
					if (rate_h == 0) {
						update(window, flag);
						window.draw(ccheck);
						*choice = 2;
						window.display();
						*check = 1;
					}
					else {
						update(window, flag);
						window.draw(ccall);
						*choice = 3;
						window.display();
						if (bank_c < (rate_h - *rate_c)) {
							bank_h += (rate_h - *rate_c) - bank_c;
							bank_all -= (rate_h - *rate_c) - bank_c;
							*rate_c += COMP_RATE(bank_c);
						}
						*rate_c += COMP_RATE(rate_h - *rate_c);
					}
				}
			}
		}
	}
	_sleep(500);
}

void RATES_BEGIN() {
	bank_all += 100;
	bank_c -= 50;
	bank_h -= 50;
	bluff = rand() % 101;
}

void RATES(RenderWindow& window, char flag) {
	char check_c = 0, check_h = 0, choice = 0, choice_c = 0;
	unsigned int rate_h = 0, rate_save = 0, rate_c = 0;

	if (dealer_chip == 1) {
		_sleep(500);
		if (flag == 1) II(window, flag, 0, &rate_c, &check_c, flop_prob[hand_card][0] + flop_prob[hand_card][1], bluff, &choice_c);
		else II(window, flag, 0, &rate_c, &check_c, 0, bluff, &choice_c);
	}
	while (1) {
		if (end_game == 2) break;
		if ((bank_c == 0 || bank_h == 0) && (rate_c <= rate_h)) break;
		rate_save = rate_h;
		while (1) {
			update(window, flag);

			switch (choice) {
			case 1: {
				window.draw(crate);
				break;
			}
			case 2: {
				window.draw(ccheck);
				break;
			}
			case 3: {
				window.draw(ccall);
				break;
			}
			case 4: {
				window.draw(cpass);
				break;
			}
			}

			/*if (choice_c == 1) window.draw(crate);
			if (choice_c == 2) window.draw(ccheck);
			if (choice_c == 3) window.draw(ccall);
			if (choice_c == 4) window.draw(cpass);*/

			print.str("");
			print << rate_c;
			rate_cchip.setString(print.str());
			window.draw(rate_cchip);
			print.str("");
			print << rate_h;
			rate_hchip.setString(print.str());
			window.draw(rate_hchip);

			if (IntRect(1272, 770, 150, 50).contains(Mouse::getPosition(window))) {
				rate.setColor(Color::Blue);
				window.draw(rate);
			}
			else {
				rate.setColor(Color::White);
				window.draw(rate);
			}
			if (IntRect(1272, 840, 150, 50).contains(Mouse::getPosition(window))) {
				if (rate_c != rate_h) {
					call.setColor(Color::Blue);
					window.draw(call);
				}
				else {
					check.setColor(Color::Blue);
					window.draw(check);
				}
			}
			else {
				if (rate_c != rate_h) {
					call.setColor(Color::White);
					window.draw(call);
				}
				else {
					check.setColor(Color::White);
					window.draw(check);
				}
			}
			if (IntRect(1272, 910, 150, 50).contains(Mouse::getPosition(window))) {
				pass.setColor(Color::Blue);
				window.draw(pass);
			}
			else {
				pass.setColor(Color::White);
				window.draw(pass);
			}
			if (IntRect(1272, 770, 150, 50).contains(Mouse::getPosition(window)) && Mouse::isButtonPressed(Mouse::Left)) {
				rate.setColor(Color::Red);
				window.draw(rate);
				window.display();
				choice = 1;
				break;
			}
			if (IntRect(1272, 840, 150, 50).contains(Mouse::getPosition(window)) && Mouse::isButtonPressed(Mouse::Left)) {
				if (rate_c != rate_h) {
					call.setColor(Color::Red);
					choice = 2;
					window.draw(call);
					window.display();
				}
				else {
					check.setColor(Color::Red);
					choice = 2;
					window.draw(check);
					window.display();
				}
				break;
			}
			if (IntRect(1272, 910, 150, 50).contains(Mouse::getPosition(window)) && Mouse::isButtonPressed(Mouse::Left)) {
				pass.setColor(Color::Red);
				choice = 3;
				window.draw(pass);
				window.display();
				break;
			}
			window.display();
		}
		_sleep(100);

		if (choice == 1) {
			if (check_c == 1) check_c = 0;
			if (rate_c - rate_h >= bank_h) {
				bank_c += (rate_c - rate_h) - bank_h;
				bank_all -= (rate_c - rate_h) - bank_h;
				bank_all += bank_h;
				bank_h = 0;
				break;
			}
			while (1) {
				rate_h = 0;
				print_rate.str("");

				short temp[4], i = -1;
				memset(temp, 0, 4);
				sf::Event event;
				while (1) {
					update(window, flag);
					hrate.setString(": " + print_rate.str());
					window.draw(hrate);
					window.draw(ok);
					window.draw(rate);
					if (choice_c == 1) window.draw(crate);
					if (choice_c == 2) window.draw(ccheck);
					if (choice_c == 3) window.draw(ccall);
					if (choice_c == 4) window.draw(cpass);

					print.str("");
					print << rate_c;
					rate_cchip.setString(print.str());
					window.draw(rate_cchip);
					print.str("");
					print << rate_save;
					rate_hchip.setString(print.str());
					window.draw(rate_hchip);

					if (IntRect(1475, 770, 150, 50).contains(Mouse::getPosition(window))) {
						ok.setColor(Color::Blue);
						window.draw(ok);
					}
					else {
						ok.setColor(Color::White);
						window.draw(ok);
					}
					if (IntRect(1475, 770, 150, 50).contains(Mouse::getPosition(window)) && Mouse::isButtonPressed(Mouse::Left)) {
						ok.setColor(Color::Red);
						window.draw(ok);
						window.display();
						break;
					}
					while (window.pollEvent(event)) {
						if (i < 3) {
							if (event.type == sf::Event::KeyPressed) {
								switch (event.key.code) {
								case Keyboard::Numpad0:
									++i;
									temp[i] = 0;
									print_rate << temp[i];
									break;
								case Keyboard::Numpad1:
									++i;
									temp[i] = 1;
									print_rate << temp[i];
									break;
								case Keyboard::Numpad2:
									++i;
									temp[i] = 2;
									print_rate << temp[i];
									break;
								case Keyboard::Numpad3:
									++i;
									temp[i] = 3;
									print_rate << temp[i];
									break;
								case Keyboard::Numpad4:
									++i;
									temp[i] = 4;
									print_rate << temp[i];
									break;
								case Keyboard::Numpad5:
									++i;
									temp[i] = 5;
									print_rate << temp[i];
									break;
								case Keyboard::Numpad6:
									++i;
									temp[i] = 6;
									print_rate << temp[i];
									break;
								case Keyboard::Numpad7:
									++i;
									temp[i] = 7;
									print_rate << temp[i];
									break;
								case Keyboard::Numpad8:
									++i;
									temp[i] = 8;
									print_rate << temp[i];
									break;
								case Keyboard::Numpad9:
									++i;
									temp[i] = 9;
									print_rate << temp[i];
									break;
								}
							}
						}
						if (i > -1) {
							if (event.type == sf::Event::KeyPressed) {
								switch (event.key.code) {
								case Keyboard::Backspace:
									temp[i] = '\0';
									i--;
									print_rate.str("");
									for (char j = 0; j <= i; j++) print_rate << temp[j];
									break;
								}
							}
						}
					}
					window.display();
				}
				char j = i;
				while (i != -1) {
					rate_h += temp[i] * pow(10, (j - i));
					i--;
				}
				if (rate_h <= bank_h && rate_h + rate_save >= rate_c) {
					bank_h -= rate_h;
					bank_all += rate_h;
					break;
				}
			}
			_sleep(100);
			if (flag == 1) II(window, flag, rate_h + rate_save, &rate_c, &check_c, flop_prob[hand_card][0] + flop_prob[hand_card][1], bluff, &choice_c);
			else II(window, flag, rate_h + rate_save, &rate_c, &check_c, 0, bluff, &choice_c);
			if (end_game == 2) break;
			if (rate_h + rate_save == rate_c) break;
			rate_h += rate_save;
		}
		else if (choice == 2) {
			choice_c = 0;
			if (check_c == 1) check_h = 1;
			if (rate_c == 0 && check_c == 0) {
				check_h = 1;
				_sleep(100);
				if (flag == 1) II(window, flag, rate_h, &rate_c, &check_c, flop_prob[hand_card][0] + flop_prob[hand_card][1], bluff, &choice_c);
				else II(window, flag, rate_h, &rate_c, &check_c, 0, bluff, &choice_c);
				if (check_h == 1 && check_c == 1) break;
				if (rate_c != 0) check_h = 0;
				continue;
			}
			if (rate_h < rate_c) {
				if (bank_h < (rate_c - rate_h)) {
					bank_c += (rate_c - rate_h) - bank_h;
					bank_all -= (rate_c - rate_h) - bank_h;
					bank_all += bank_h;
					bank_h = 0;
				}
				else {
					bank_all += rate_c - rate_h;
					bank_h -= rate_c - rate_h;
				}
				break;
			}
			if (check_h == 1 && check_c == 1) break;
			if (end_game == 2) break;
		}
		else if (choice == 3) {
			choice_c = 0;
			end_game = 1;
			break;
		}
	}
}

void ENDGAME() {
	if (end_game == 1) {
		bank_c += bank_all;
	}
	if (end_game == 2) {
		bank_h += bank_all;
	}
	if (dealer_chip == 0) dealer_chip = 1;
	else dealer_chip = 0;
}

void START_HAND() {
	char sort;

	card_c[0] = 1 + rand() % 52;
	while (1) {
		card_h[0] = 1 + rand() % 52;
		if (card_h[0] != card_c[0]) break;
	}
	while (1) {
		card_c[1] = 1 + rand() % 52;
		if (card_c[1] != card_c[0] && card_c[1] != card_h[0]) break;
	}
	while (1) {
		card_h[1] = 1 + rand() % 52;
		if (card_h[1] != card_c[0] && card_h[1] != card_h[0] && card_h[1] != card_c[1]) break;
	}

	if ((((card_c[0] % 13) > (card_c[1] % 13)) && (card_c[1] % 13) != 0) || (card_c[0] % 13) == 0) {
		sort = card_c[0];
		card_c[0] = card_c[1];
		card_c[1] = sort;
	}
	if ((((card_h[0] % 13) > (card_h[1] % 13)) && (card_h[1] % 13) != 0) || (card_h[0] % 13) == 0) {
		sort = card_h[0];
		card_h[0] = card_h[1];
		card_h[1] = sort;
	}

}

void FLOP() { // первые три карты
	char flag;

	while (1) {
		flag = 1;
		flop[0] = 1 + rand() % 52;

		if (flop[0] == card_c[0] || flop[0] == card_h[0] || flop[0] == card_c[1] || flop[0] == card_h[1]) flag = 0;

		if (flag == 1) break;
	}

	while (1) {
		flag = 1;
		flop[1] = 1 + rand() % 52;

		if (flop[1] == card_c[0] || flop[1] == card_h[0] || flop[1] == card_c[1] || flop[1] == card_h[1]) flag = 0;

		if (flag == 1 && flop[1] != flop[0]) break;
	}

	while (1) {
		flag = 1;
		flop[2] = 1 + rand() % 52;

		if (flop[2] == card_c[0] || flop[2] == card_h[0] || flop[2] == card_c[1] || flop[2] == card_h[1]) flag = 0;

		if (flag == 1 && flop[2] != flop[0] && flop[2] != flop[1]) break;
	}

}

void TURN() {
	char flag;

	while (1) {
		flag = 1;
		turn = 1 + rand() % 52;

		if (turn == card_c[0] || turn == card_h[0] || turn == card_c[1] || turn == card_h[1]) flag = 0;

		if (flag == 1) if ((turn == flop[0]) || (turn == flop[1]) || (turn == flop[2])) flag = 0;

		if (flag == 1) break;
	}

	turn_flag = 1;

}

void RIVER() {
	char flag;

	while (1) {
		flag = 1;
		river = 1 + rand() % 52;

		if (river == card_c[0] || river == card_h[0] || river == card_c[1] || river == card_h[1]) flag = 0;

		if (flag == 1)  if ((river == flop[0]) || (river == flop[1]) || (river == flop[2])) flag = 0;

		if (flag == 1 && river != turn) break;
	}

	river_flag = 1;

}

void pairandsettableHELP(char x, char y) {

	if (x == y) {
		if (pair_table[x] == 0) {
			if (set_table[x] == 0) pair_table[x] = 1;
		}
		else {
			pair_table[x] = 0;
			set_table[x] = 1;
		}
	}

}

void PAIR_AND_SET_TABLE(char Tflag, char Rflag) {

	if (Tflag == 0 && Rflag == 0) {
		memset(pair_table, 0, 13);
		memset(set_table, 0, 13);
		if ((flop[0] % 13) == (flop[1] % 13)) pair_table[flop[0] % 13] = 1;
		if ((flop[0] % 13) == (flop[2] % 13)) pair_table[flop[0] % 13] = 1;
		if ((flop[1] % 13) == (flop[2] % 13)) pair_table[flop[1] % 13] = 1;
		if ((flop[0] % 13) == (flop[1] % 13) && (flop[0] % 13) == (flop[2] % 13)) {
			pair_table[flop[0] % 13] = 0;
			set_table[flop[0] % 13] = 1;
		}
	}

	if (Tflag == 1 && Rflag == 0) {
		pairandsettableHELP(turn % 13, flop[0] % 13);
		pairandsettableHELP(turn % 13, flop[1] % 13);
		pairandsettableHELP(turn % 13, flop[2] % 13);
	}


	if (Tflag == 1 && Rflag == 1) {
		pairandsettableHELP(river % 13, flop[0] % 13);
		pairandsettableHELP(river % 13, flop[1] % 13);
		pairandsettableHELP(river % 13, flop[2] % 13);
		pairandsettableHELP(river % 13, turn % 13);
	}

}

void straightHELP(char num, char dro) {
	char count_0 = 0, count_1 = 0;

	for (char i = num; i < num + 5; i++) {
		if (i == 13) {
			if (straight[0] == 1 || straight[0] == 2) ++count_1;
			if (straight[0] == 0) ++count_0;
			break;
		}
		if (straight[i] == 1 || straight[i] == 2) ++count_1;
		if (straight[i] == 0) ++count_0;
	}
	if (dro == 0) if (count_1 > 2 && count_0 < 3) real_straight[num] = 1;
	if (dro == 1) {
		if (turn_flag == 0 && river_flag == 0) if (count_1 > 2 && count_1 < 5 && count_0 < 3) real_straightDRO[num] = 1;
		if (turn_flag == 1 && river_flag == 0) if (count_1 == 4 && count_0 == 1) real_straightDRO[num] = 1;
	}

}

void STRAIGHT() {

	if (turn_flag == 0 && river_flag == 0) {
		memset(straight, 0, 13);
		straight[flop[0] % 13] = 1;
		straight[flop[1] % 13] = 1;
		straight[flop[2] % 13] = 1;
	}

	if (turn_flag == 1 && river_flag == 0) straight[turn % 13] = 1;

	if (turn_flag == 1 && river_flag == 1) straight[river % 13] = 1;

	for (char i = 0; i < 10; i += 5) {
		straightHELP(i, 0);
		straightHELP(i + 1, 0);
		straightHELP(i + 2, 0);
		straightHELP(i + 3, 0);
		straightHELP(i + 4, 0);
	}

}

void flashHELP(char* flush, char* flag, char dro) {

	for (char i = 0; i < 13; i++) if (flush[i] == 1 || flush[i] == 2) *flag = *flag + 1;
	if (dro == 0) {
		if (*flag >= 3) *flag = 1;
		else *flag = 0;
	}
	if (dro == 1) {
		if (turn_flag == 0 && river_flag == 0) {
			if (*flag >= 3 && *flag < 5) {
				outs_flush = 11 - *flag;
				*flag = 1;
			}
			else *flag = 0;
		}
		if (turn_flag == 1 && river_flag == 0) {
			if (*flag == 4) {
				outs_flush = 7;
				*flag = 1;
			}
			else *flag = 0;
		}
	}

}

void FLASH() {
	DFflag = 0; CFflag = 0; HFflag = 0; SFflag = 0;

	if (turn_flag == 0 && river_flag == 0) {
		memset(Dflash, 0, 13);
		memset(Cflash, 0, 13);
		memset(Hflash, 0, 13);
		memset(Sflash, 0, 13);
		for (char i = 0; i < 3; i++) {
			if (flop[i] < 14) Dflash[flop[i] % 13] = 1;
			else if (flop[i] > 13 && flop[i] < 27) Cflash[flop[i] % 13] = 1;
			else if (flop[i] > 26 && flop[i] < 40) Hflash[flop[i] % 13] = 1;
			else if (flop[i] > 39 && flop[i] < 53) Sflash[flop[i] % 13] = 1;
		}
		flashHELP(Dflash, &DFflag, 0);
		flashHELP(Cflash, &CFflag, 0);
		flashHELP(Hflash, &HFflag, 0);
		flashHELP(Sflash, &SFflag, 0);
	}

	if (turn_flag == 1 && river_flag == 0) {
		if (turn < 14) Dflash[turn % 13] = 1;
		else if (turn > 13 && turn < 27) Cflash[turn % 13] = 1;
		else if (turn > 26 && turn < 40) Hflash[turn % 13] = 1;
		else if (turn > 39 && turn < 53) Sflash[turn % 13] = 1;
		flashHELP(Dflash, &DFflag, 0);
		flashHELP(Cflash, &CFflag, 0);
		flashHELP(Hflash, &HFflag, 0);
		flashHELP(Sflash, &SFflag, 0);
	}

	if (turn_flag == 1 && river_flag == 1) {
		if (river < 14) Dflash[river % 13] = 1;
		else if (river > 13 && river < 27) Cflash[river % 13] = 1;
		else if (river > 26 && river < 40) Hflash[river % 13] = 1;
		else if (river > 39 && river < 53) Sflash[river % 13] = 1;
		flashHELP(Dflash, &DFflag, 0);
		flashHELP(Cflash, &CFflag, 0);
		flashHELP(Hflash, &HFflag, 0);
		flashHELP(Sflash, &SFflag, 0);
	}

}

void CHECK_STRAIGHTDRO() {

	if (straight[card_c[0] % 13] == 0) straight[card_c[0] % 13] = 2;
	if (straight[card_c[1] % 13] == 0) straight[card_c[1] % 13] = 2;

	memset(real_straightDRO, 0, 13);

	for (char i = 0; i < 10; i += 5) {
		straightHELP(i, 1);
		straightHELP(i + 1, 1);
		straightHELP(i + 2, 1);
		straightHELP(i + 3, 1);
		straightHELP(i + 4, 1);
	}

	memset(outST, 0, 13);
	for (char i = 0; i < 10; i++) if (real_straightDRO[i] == 1) {
		for (char j = i; j < (i + 5); j++) {
			if (j == 13) {
				if (straight[0] == 0) outST[0] = 1;
				break;
			}
			if (straight[j] == 0) outST[j] = 1;
		}
	}

	for (char i = 0; i < 13; i++) if (straight[i] == 2) straight[i] = 0;
}

void CHECK_FLUSHDRO() {
	DFDflag = 0; CFDflag = 0; HFDflag = 0; SFDflag = 0;

	if (card_c[0] < 14 && Dflash[card_c[0] % 13] == 0) Dflash[card_c[0] % 13] = 2;
	else if (card_c[0] > 13 && card_c[0] < 27 && Cflash[card_c[0] % 13] == 0) Cflash[card_c[0] % 13] = 2;
	else if (card_c[0] > 26 && card_c[0] < 40 && Hflash[card_c[0] % 13] == 0) Hflash[card_c[0] % 13] = 2;
	else if (card_c[0] > 39 && card_c[0] < 53 && Sflash[card_c[0] % 13] == 0) Sflash[card_c[0] % 13] = 2;

	if (card_c[1] < 14 && Dflash[card_c[1] % 13] == 0) Dflash[card_c[1] % 13] = 2;
	else if (card_c[1] > 13 && card_c[1] < 27 && Cflash[card_c[1] % 13] == 0) Cflash[card_c[1] % 13] = 2;
	else if (card_c[1] > 26 && card_c[1] < 40 && Hflash[card_c[1] % 13] == 0) Hflash[card_c[1] % 13] = 2;
	else if (card_c[1] > 39 && card_c[1] < 53 && Sflash[card_c[1] % 13] == 0) Sflash[card_c[1] % 13] = 2;

	flashHELP(Dflash, &DFDflag, 1);
	flashHELP(Cflash, &CFDflag, 1);
	flashHELP(Hflash, &HFDflag, 1);
	flashHELP(Sflash, &SFDflag, 1);

	for (char i = 0; i < 13; i++) {
		if (Dflash[i] == 2) Dflash[i] = 0;
		else if (Cflash[i] == 2) Cflash[i] = 0;
		else if (Hflash[i] == 2) Hflash[i] = 0;
		else if (Sflash[i] == 2) Sflash[i] = 0;
	}
}

void PROBABILITY() {

	if (turn_flag == 0 && river_flag == 0) {
		outs_straight = 0;
		for (char i = 0; i < 13; i++) if (outST[i] == 1) ++outs_straight;
	}

	if (turn_flag == 1 && river_flag == 0) {
		outs_straight = 0;
		for (char i = 0; i < 13; i++) if (outST[i] == 1) ++outs_straight;
	}

}

void CHECK_PAIR(char* combo) {
	if (pair_table[0] == 1) combo[0] = 0;
	for (char i = 12; i > 0; i -= 3) {
		if (pair_table[i] == 1 && combo[0] != -1) {
			combo[1] = i;
			break;
		}
		if (pair_table[i] == 1) combo[0] = i;

		if (pair_table[i + 1] == 1 && combo[0] != -1) {
			combo[1] = i + 1;
			break;
		}
		if (pair_table[i + 1] == 1) combo[0] = i + 1;

		if (pair_table[i + 2] == 1 && combo[0] != -1) {
			combo[1] = i + 2;
			break;
		}
		if (pair_table[i + 2] == 1) combo[0] = i + 2;
	}
}

void CHECK_THREE(char* combo, char card_1, char card_2, char flag) {
	char check = 0;

	if (set_table[0] == 1) {
		if ((flop[0] % 13) == 0) ++check;
		if ((flop[1] % 13) == 0) ++check;

		if ((turn % 13) == 0) ++check;
		if ((river % 13) == 0) ++check;
		if (flag == 1) {
			if (card_1 % 13 == 0) ++check;
			if (card_2 % 13 == 0) ++check;
		}
		if (check == 4) combo[6] = 0;
		else combo[2] = 0;
	}

	for (char i = 12; i > 0; i -= 3) {
		if (set_table[i] == 1) {
			for (char j = 0; j < 3; j++) if ((flop[j] % 13) == i) ++check;

			if ((flop[0] % 13) == i) ++check;
			if ((flop[1] % 13) == i) ++check;
			if ((flop[2] % 13) == i) ++check;

			if ((turn % 13) == i) ++check;
			if ((river % 13) == i) ++check;
			if (flag == 1) {
				if ((river % 13) == i) ++check;
				if (card_1 % 13 == i) ++check;
				if (card_2 % 13 == i) ++check;
			}
			if (check == 4) combo[6] = i;
			else {
				if (combo[2] == -1) combo[2] = i;
				else {
					combo[9] = i;
					break;
				}
			}
			if (flag == 0) break;
		}

		if (set_table[i - 1] == 1) {
			for (char j = 0; j < 3; j++) if ((flop[j] % 13) == i - 1) ++check;

			if ((flop[0] % 13) == i - 1) ++check;
			if ((flop[1] % 13) == i - 1) ++check;
			if ((flop[2] % 13) == i - 1) ++check;

			if ((turn % 13) == i - 1) ++check;
			if ((river % 13) == i - 1) ++check;
			if (flag == 1) {
				if ((river % 13) == i - 1) ++check;
				if (card_1 % 13 == i - 1) ++check;
				if (card_2 % 13 == i - 1) ++check;
			}
			if (check == 4) combo[6] = i - 1;
			else {
				if (combo[2] == -1) combo[2] = i - 1;
				else {
					combo[9] = i - 1;
					break;
				}
			}
			if (flag == 0) break;
		}

		if (set_table[i - 2] == 1) {
			for (char j = 0; j < 3; j++) if ((flop[j] % 13) == i - 2) ++check;

			if ((flop[0] % 13) == i - 2) ++check;
			if ((flop[1] % 13) == i - 2) ++check;
			if ((flop[2] % 13) == i - 2) ++check;

			if ((turn % 13) == i - 2) ++check;
			if ((river % 13) == i - 2) ++check;
			if (flag == 1) {
				if ((river % 13) == i - 2) ++check;
				if (card_1 % 13 == i - 2) ++check;
				if (card_2 % 13 == i - 2) ++check;
			}
			if (check == 4) combo[6] = i - 2;
			else {
				if (combo[2] == -1) combo[2] = i - 2;
				else {
					combo[9] = i - 2;
					break;
				}
			}
			if (flag == 0) break;
		}
	}
}

void CHECK_STRAIGHT(char* combo) {
	char check = 0;

	for (char i = 9; i >= 0; i--) {
		if (real_straight[i] == 1) {
			for (char j = i; j < (i + 5); j++) {
				if (j == 13) {
					if (straight[0] == 1 || straight[0] == 2) ++check;
					break;
				}
				if (straight[j] == 1 || straight[j] == 2) ++check;
				else {
					check = 0;
					break;
				}
			}
			if (check == 5) {
				combo[3] = i;
				break;
			}
			else check = 0;
		}
	}

}

void CHECK_FLUSH(char* combo, char* flush) {
	char check = 0;

	for (char i = 0; i < 13; i++) if (flush[i] == 1 || flush[i] == 2) ++check;
	if (check >= 5) {
		if (flush[0] == 1 || flush[0] == 2) combo[4] = 0;
		else {
			for (char i = 12; i > 0; i--) {
				if (flush[i] == 1 || flush[i] == 2) {
					combo[4] = i;
					break;
				}
			}
		}
	}

}

void CHECK_STRAIGHTFLUSH(char* combo, char* flush) {
	char check = 0;

	for (char i = 9; i >= 0; i--) {
		if (flush[i] == 1 || flush[i] == 2) {
			for (char j = i; j < (i + 5); j++) {
				if (j == 13) {
					if (flush[0] == 1 || flush[0] == 2) ++check;
					break;
				}
				if (flush[j] == 1 || flush[j] == 2) ++check;
				else {
					check = 0;
					break;
				}
			}
			if (check == 5) {
				combo[7] = i;
				break;
			}
		}
	}
}

void CHECK_TABLE() {

	memset(TCombination, -1, 9);

	CHECK_PAIR(TCombination);

	CHECK_THREE(TCombination, -1, -1, 0);

	CHECK_STRAIGHT(TCombination);

	if (DFflag == 1) CHECK_FLUSH(TCombination, Dflash);
	if (CFflag == 1) CHECK_FLUSH(TCombination, Cflash);
	if (HFflag == 1) CHECK_FLUSH(TCombination, Hflash);
	if (SFflag == 1) CHECK_FLUSH(TCombination, Sflash);

	if (TCombination[0] != -1 && TCombination[2] != -1) TCombination[5] = TCombination[2];

	if (TCombination[3] != -1 && TCombination[4] != -1) {
		if (TCombination[3] == 9) TCombination[8] = 0;
		else TCombination[7] = TCombination[3];
	}

}

void CHECK_HAND(char* combo, char* card) {

	memset(combo, -1, 10);

	pairandsettableHELP(card[0] % 13, card[1] % 13);

	pairandsettableHELP(card[0] % 13, flop[0] % 13);
	pairandsettableHELP(card[0] % 13, flop[1] % 13);
	pairandsettableHELP(card[0] % 13, flop[2] % 13);
	if (turn_flag == 1 && river_flag == 0) pairandsettableHELP(card[0] % 13, turn % 13);
	if (turn_flag == 1 && river_flag == 1) {
		pairandsettableHELP(card[0] % 13, turn % 13);
		pairandsettableHELP(card[0] % 13, river % 13);
	}

	pairandsettableHELP(card[1] % 13, flop[0] % 13);
	pairandsettableHELP(card[1] % 13, flop[1] % 13);
	pairandsettableHELP(card[1] % 13, flop[2] % 13);
	if (turn_flag == 1 && river_flag == 0) pairandsettableHELP(card[1] % 13, turn % 13);
	if (turn_flag == 1 && river_flag == 1) {
		pairandsettableHELP(card[1] % 13, turn % 13);
		pairandsettableHELP(card[1] % 13, river % 13);
	}

	CHECK_PAIR(combo);

	CHECK_THREE(combo, card[0], card[1], 1);

	for (char i = 0; i < 2; i++) if (straight[card[i] % 13] == 0) straight[card[i] % 13] = 2;

	CHECK_STRAIGHT(combo);

	if (DFflag == 1) {
		if (card[0] < 14 && Dflash[card[0] % 13] == 0) Dflash[card[0] % 13] = 2;
		if (card[1] < 14 && Dflash[card[1] % 13] == 0) Dflash[card[1] % 13] = 2;
		CHECK_FLUSH(combo, Dflash);
	}
	if (CFflag == 1) {
		if (card[0] > 13 && card[0] < 27 && Cflash[card[0] % 13] == 0) Cflash[card[0] % 13] = 2;
		if (card[1] > 13 && card[1] < 27 && Cflash[card[1] % 13] == 0) Cflash[card[1] % 13] = 2;
		CHECK_FLUSH(combo, Cflash);
	}
	if (HFflag == 1) {
		if (card[0] > 26 && card[0] < 40 && Hflash[card[0] % 13] == 0) Hflash[card[0] % 13] = 2;
		if (card[1] > 26 && card[1] < 40 && Hflash[card[1] % 13] == 0) Hflash[card[1] % 13] = 2;
		CHECK_FLUSH(combo, Hflash);
	}
	if (SFflag == 1) {
		if (card[0] > 39 && card[0] < 53 && Sflash[card[0] % 13] == 0) Sflash[card[0] % 13] = 2;
		if (card[1] > 39 && card[1] < 53 && Sflash[card[1] % 13] == 0) Sflash[card[1] % 13] = 2;
		CHECK_FLUSH(combo, Sflash);
	}

	if (combo[2] != -1) if (combo[0] != -1 || combo[9] != -1) combo[5] = combo[2];

	if (combo[3] != -1 && combo[4] != -1) {
		if (DFflag == 1) CHECK_STRAIGHTFLUSH(combo, Dflash);
		if (CFflag == 1) CHECK_STRAIGHTFLUSH(combo, Cflash);
		if (HFflag == 1) CHECK_STRAIGHTFLUSH(combo, Hflash);
		if (SFflag == 1) CHECK_STRAIGHTFLUSH(combo, Sflash);
	}

	if (combo[7] == 9) combo[8] = 0;

	PAIR_AND_SET_TABLE(0, 0);
	if (turn_flag == 1 && river_flag == 0) PAIR_AND_SET_TABLE(1, 0);
	if (turn_flag == 1 && river_flag == 1) {
		PAIR_AND_SET_TABLE(1, 0);
		PAIR_AND_SET_TABLE(1, 1);
	}

	for (char i = 0; i < 13; i++) if (straight[i] == 2) straight[i] = 0;

	if (DFflag == 1) for (char i = 0; i < 13; i++) if (Dflash[i] == 2) Dflash[i] = 0;
	if (CFflag == 1) for (char i = 0; i < 13; i++) if (Cflash[i] == 2) Cflash[i] = 0;
	if (HFflag == 1) for (char i = 0; i < 13; i++) if (Hflash[i] == 2) Hflash[i] = 0;
	if (SFflag == 1) for (char i = 0; i < 13; i++) if (Sflash[i] == 2) Sflash[i] = 0;

}

void qsortRecursive(char* mas, char size) {
	int i = 0;
	int j = size - 1;
	int mid = mas[size >> 1];
	do {
		while (mas[i] < mid) ++i;
		while (mas[j] > mid) j--;
		if (i <= j) {
			int tmp = mas[i];
			mas[i] = mas[j];
			mas[j] = tmp;
			++i;
			j--;
		}
	} while (i <= j);
	if (j > 0) qsortRecursive(mas, j + 1);
	if (i < size) qsortRecursive(&mas[i], size - i);
}

char WINNER(RenderWindow& window) {
	char winC = -1, winH = -1;
	char Ccard[7], Hcard[7];
	memset(Ccard, -1, 7);
	memset(Hcard, -1, 7);

	for (char i = 8; i >= 0; i--) {
		if (HCCombination[i] != -1 && i > winC) winC = i;
		if (HHCombination[i] != -1 && i > winH) winH = i;
	}

	if (winC > winH) {
		bank_c += bank_all;
		return 1;
	}
	if (winC < winH) {
		bank_h += bank_all;
		return 2;
	}
	if (winC == winH) {
		if (winC == -1) {
			Ccard[0] = flop[0] % 13;
			Hcard[0] = flop[0] % 13;

			Ccard[1] = flop[1] % 13;
			Hcard[1] = flop[1] % 13;

			Ccard[2] = flop[2] % 13;
			Hcard[2] = flop[2] % 13;

			Ccard[3] = turn % 13; Hcard[3] = turn % 13;
			Ccard[4] = river % 13; Hcard[4] = river % 13;
			Ccard[5] = card_c[0] % 13; Hcard[5] = card_h[0] % 13;
			Ccard[6] = card_c[1] % 13; Hcard[6] = card_h[1] % 13;
			for (char i = 0; i < 7; i++) {
				if (Ccard[i] == 0) Ccard[i] = 13;
				if (Hcard[i] == 0) Hcard[i] = 13;
			}
			qsortRecursive(Ccard, 7);
			qsortRecursive(Hcard, 7);
			for (char i = 6; i > 1; i--) {
				if (Ccard[i] > Hcard[i]) {
					bank_c += bank_all;
					return 1;
				}
				if (Ccard[i] < Hcard[i]) {
					bank_h += bank_all;
					return 2;
				}
				if (Ccard[i] == Hcard[i] && i == 2) {
					bank_c += (bank_all >> 1);
					bank_h += (bank_all >> 1);
					return 3;
				}
			}
		}
		if (winC == 0) {
			if (HCCombination[0] == 0) HCCombination[0] = 13;
			if (HHCombination[0] == 0) HHCombination[0] = 13;
			if (HCCombination[0] > HHCombination[0]) {
				bank_c += bank_all;
				return 1;
			}
			if (HCCombination[0] < HHCombination[0]) {
				bank_h += bank_all;
				return 2;
			}
			if (HCCombination[0] == HHCombination[0]) {
				if (HCCombination[0] == 13) HCCombination[0] = 0;
				if (HHCombination[0] == 13) HHCombination[0] = 0;

				if (flop[0] % 13 != HCCombination[0]) {
					Ccard[0] = flop[0] % 13;
					Hcard[0] = flop[0] % 13;
				}
				if (flop[1] % 13 != HCCombination[0]) {
					Ccard[1] = flop[1] % 13;
					Hcard[1] = flop[1] % 13;
				}
				if (flop[2] % 13 != HCCombination[0]) {
					Ccard[2] = flop[2] % 13;
					Hcard[2] = flop[2] % 13;
				}

				if (turn % 13 != HCCombination[0]) {
					Ccard[3] = turn % 13;
					Hcard[3] = turn % 13;
				}
				if (river % 13 != HCCombination[0]) {
					Ccard[4] = river % 13;
					Hcard[4] = river % 13;
				}
				if (card_c[0] % 13 != HCCombination[0]) Ccard[5] = card_c[0] % 13;
				if (card_c[1] % 13 != HCCombination[0]) Ccard[6] = card_c[1] % 13;

				if (card_h[0] % 13 != HCCombination[0]) Hcard[5] = card_h[0] % 13;
				if (card_h[1] % 13 != HCCombination[0]) Hcard[6] = card_h[1] % 13;

				for (char i = 0; i < 7; i++) {
					if (Ccard[i] == 0) Ccard[i] = 13;
					if (Hcard[i] == 0) Hcard[i] = 13;
				}
				qsortRecursive(Ccard, 7);
				qsortRecursive(Hcard, 7);
				/*for (char i = 6; i > 3; i--) {
					if (Ccard[i] > Hcard[i]) {
						bank_c += bank_all;
						return 1;
					}
					if (Ccard[i] < Hcard[i]) {
						bank_h += bank_all;
						return 2;
					}
					if (Ccard[i] == Hcard[i] && i == 4) {
						bank_c += bank_all >> 1;
						bank_h += bank_all >> 1;
						return 3;
					}
				}*/
				if ((Ccard[6] > Hcard[6]) || (Ccard[5] > Hcard[5]) || (Ccard[4] > Hcard[4])) {
					bank_c += bank_all;
					return 1;
				}
				else if ((Ccard[6] < Hcard[6]) || (Ccard[5] < Hcard[5]) || (Ccard[4] < Hcard[4])) {
					bank_h += bank_all;
					return 2;
				}
				else if ((Ccard[6] == Hcard[6]) || (Ccard[5] == Hcard[5]) || (Ccard[4] == Hcard[4])) {
					bank_c += bank_all >> 1;
					bank_h += bank_all >> 1;
					return 3;
				}
			}
		}
		if (winC == 1) {
			if (HCCombination[0] == 0) HCCombination[0] = 13;
			if (HHCombination[0] == 0) HHCombination[0] = 13;
			if (HCCombination[0] > HHCombination[0]) {
				bank_c += bank_all;
				return 1;
			}
			if (HCCombination[0] < HHCombination[0]) {
				bank_h += bank_all;
				return 2;
			}
			if (HCCombination[0] == HHCombination[0]) {
				if (HCCombination[0] == 13) HCCombination[0] = 0;
				if (HHCombination[0] == 13) HHCombination[0] = 0;
				if (HCCombination[1] > HHCombination[1]) {
					bank_c += bank_all;
					return 1;
				}
				if (HCCombination[1] < HHCombination[1]) {
					bank_h += bank_all;
					return 2;
				}
				if (HCCombination[1] == HHCombination[1]) {
					/*for (char i = 0; i < 3; i++) {
						if (flop[i] % 13 == 0 && HCCombination[0] != 0 && HCCombination[1] != 0) {
							bank_c += bank_all >> 1;
							bank_h += bank_all >> 1;
							return 3;
						}
						if (flop[i] % 13 != HCCombination[0] && flop[i] % 13 != HCCombination[1] && flop[i] % 13 > Ccard[0])
							Ccard[0] = flop[i] % 13;
					}*/

					if ((flop[0] % 13 == 0 && HCCombination[0] != 0 && HCCombination[1] != 0) ||
						(flop[1] % 13 == 0 && HCCombination[0] != 0 && HCCombination[1] != 0) ||
						(flop[2] % 13 == 0 && HCCombination[0] != 0 && HCCombination[1] != 0)) {
						bank_c += bank_all >> 1;
						bank_h += bank_all >> 1;
						return 3;
					}
					if (flop[0] % 13 != HCCombination[0] && flop[0] % 13 != HCCombination[1] && flop[0] % 13 > Ccard[0]) Ccard[0] = flop[0] % 13;
					if (flop[1] % 13 != HCCombination[0] && flop[1] % 13 != HCCombination[1] && flop[1] % 13 > Ccard[0]) Ccard[0] = flop[1] % 13;
					if (flop[2] % 13 != HCCombination[0] && flop[2] % 13 != HCCombination[1] && flop[2] % 13 > Ccard[0]) Ccard[0] = flop[2] % 13;

					if (turn % 13 == 0 && HCCombination[0] != 0 && HCCombination[1] != 0) {
						bank_c += bank_all >> 1;
						bank_h += bank_all >> 1;
						return 3;
					}
					if (turn % 13 != HCCombination[0] && turn % 13 != HCCombination[1] && turn % 13 > Ccard[0])
						Ccard[0] = turn % 13;
					if (river % 13 == 0 && HCCombination[0] != 0 && HCCombination[1] != 0) {
						bank_c += bank_all >> 1;
						bank_h += bank_all >> 1;
						return 3;
					}
					if (river % 13 != HCCombination[0] && river % 13 != HCCombination[1] && river % 13 > Ccard[0])
						Ccard[0] = river % 13;
					Hcard[0] = Ccard[0];
					/*for (char i = 0; i < 2; i++) {
						if (card_c[i] % 13 == 0 && HCCombination[0] != 0 && HCCombination[1] != 0) Ccard[0] = 13;
						if (card_h[i] % 13 == 0 && HCCombination[0] != 0 && HCCombination[1] != 0) Hcard[0] = 13;
						if (card_c[i] % 13 != HCCombination[0] && card_c[i] % 13 != HCCombination[1] && card_c[i] % 13 > Ccard[0])
							Ccard[0] = card_c[i] % 13;
						if (card_h[i] % 13 != HCCombination[0] && card_h[i] % 13 != HCCombination[1] && card_h[i] % 13 > Hcard[0])
							Hcard[0] = card_h[i] % 13;
					}*/

					if ((card_c[0] % 13 == 0 && HCCombination[0] != 0 && HCCombination[1] != 0) ||
						(card_c[1] % 13 == 0 && HCCombination[0] != 0 && HCCombination[1] != 0)) Ccard[0] = 13;
					if ((card_h[0] % 13 == 0 && HCCombination[0] != 0 && HCCombination[1] != 0) ||
						(card_h[1] % 13 == 0 && HCCombination[0] != 0 && HCCombination[1] != 0)) Hcard[0] = 13;

					if (card_c[0] % 13 != HCCombination[0] && card_c[0] % 13 != HCCombination[1] && card_c[0] % 13 > Ccard[0])
						Ccard[0] = card_c[0] % 13;
					if (card_c[1] % 13 != HCCombination[0] && card_c[1] % 13 != HCCombination[1] && card_c[1] % 13 > Ccard[0])
						Ccard[0] = card_c[1] % 13;

					if (card_h[0] % 13 != HCCombination[0] && card_h[1] % 13 != HCCombination[1] && card_h[0] % 13 > Hcard[0])
						Hcard[0] = card_h[0] % 13;
					if (card_h[1] % 13 != HCCombination[0] && card_h[1] % 13 != HCCombination[1] && card_h[1] % 13 > Hcard[0])
						Hcard[0] = card_h[1] % 13;

					if (Ccard[0] > Hcard[0]) {
						bank_c += bank_all;
						return 1;
					}
					if (Ccard[0] < Hcard[0]) {
						bank_h += bank_all;
						return 2;
					}
					if (Ccard[0] == Hcard[0]) {
						bank_c += bank_all >> 1;
						bank_h += bank_all >> 1;
						return 3;
					}
				}
			}
		}
		if (winC == 2) {
			if (HCCombination[2] == 0) HCCombination[2] = 13;
			if (HHCombination[2] == 0) HHCombination[2] = 13;
			if (HCCombination[2] > HHCombination[2]) {
				bank_c += bank_all;
				return 1;
			}
			if (HCCombination[2] < HHCombination[2]) {
				bank_h += bank_all;
				return 2;
			}
			if (HCCombination[2] == HHCombination[2]) {
				if (HCCombination[2] == 13) HCCombination[2] = 0;
				if (HHCombination[2] == 13) HHCombination[2] = 0;
				/*for (char i = 0; i < 3; i++) {
					if (flop[i] % 13 != HCCombination[2]) {
						Ccard[i] = flop[i] % 13;
						Hcard[i] = flop[i] % 13;
					}
				}*/
				if (flop[0] % 13 != HCCombination[2]) {
					Ccard[0] = flop[0] % 13;
					Hcard[0] = flop[0] % 13;
				}

				if (flop[1] % 13 != HCCombination[2]) {
					Ccard[1] = flop[1] % 13;
					Hcard[1] = flop[1] % 13;
				}

				if (flop[2] % 13 != HCCombination[2]) {
					Ccard[2] = flop[2] % 13;
					Hcard[2] = flop[2] % 13;
				}

				if (turn % 13 != HCCombination[2]) {
					Ccard[3] = turn % 13;
					Hcard[3] = turn % 13;
				}
				if (river % 13 != HCCombination[2]) {
					Ccard[4] = river % 13;
					Hcard[4] = river % 13;
				}

				if (card_c[0] % 13 != HCCombination[2]) Ccard[5] = card_c[0] % 13;
				if (card_c[1] % 13 != HCCombination[2]) Ccard[6] = card_c[1] % 13;

				if (card_h[0] % 13 != HCCombination[2]) Hcard[5] = card_h[0] % 13;
				if (card_h[1] % 13 != HCCombination[2]) Hcard[6] = card_h[1] % 13;

				for (char i = 0; i < 7; i++) {
					if (Ccard[i] == 0) Ccard[i] = 13;
					if (Hcard[i] == 0) Hcard[i] = 13;
				}
				qsortRecursive(Ccard, 7);
				qsortRecursive(Hcard, 7);
				/*for (char i = 6; i > 4; i--) {
					if (Ccard[i] > Hcard[i]) {
						bank_c += bank_all;
						return 1;
					}
					if (Ccard[i] < Hcard[i]) {
						bank_h += bank_all;
						return 2;
					}
					if (Ccard[i] == Hcard[i] && i == 5) {
						bank_c += bank_all >> 1;
						bank_h += bank_all >> 1;
						return 3;
					}
				}*/
				if ((Ccard[6] > Hcard[6]) || (Ccard[5] > Hcard[5])) {
					bank_c += bank_all;
					return 1;
				}
				else if ((Ccard[6] < Hcard[5]) || (Ccard[5] < Hcard[5])) {
					bank_h += bank_all;
					return 2;
				}
				else if ((Ccard[6] == Hcard[6]) || (Ccard[5] == Hcard[5])) {
					bank_c += bank_all >> 1;
					bank_h += bank_all >> 1;
					return 3;
				}
			}
		}
		if (winC == 3) {
			if (HCCombination[3] > HHCombination[3]) {
				bank_c += bank_all;
				return 1;
			}
			if (HCCombination[3] < HHCombination[3]) {
				bank_h += bank_all;
				return 2;
			}
			if (HCCombination[3] == HHCombination[3]) {
				bank_c += bank_all >> 1;
				bank_h += bank_all >> 1;
				return 3;
			}
		}
		if (winC == 4) {
			if (HCCombination[4] == 0) HCCombination[4] = 13;
			if (HHCombination[4] == 0) HHCombination[4] = 13;
			if (HCCombination[4] > HHCombination[4]) {
				bank_c += bank_all;
				return 1;
			}
			if (HCCombination[4] < HHCombination[4]) {
				bank_h += bank_all;
				return 2;
			}
			if (HCCombination[4] == HHCombination[4]) {
				if (HCCombination[4] == 13) HCCombination[4] = 0;
				if (HHCombination[4] == 13) HHCombination[4] = 0;
				char count = 0;
				if (DFflag == 1) {
					for (char i = 0; i < 13; i++) if (Dflash[i] == 1) {
						Ccard[count] = i;
						Hcard[count] = i;
						++count;
					}
					/*for (char i = 0; i < 2; i++) if (card_c[i] < 14 && Dflash[card_c[i] % 13] == 0)
						Ccard[count + i] = card_c[i] % 13;*/
					if (card_c[0] < 14 && Dflash[card_c[0] % 13] == 0) Ccard[count] = card_c[0] % 13;
					if (card_c[1] < 14 && Dflash[card_c[1] % 13] == 0) Ccard[count + 1] = card_c[1] % 13;

					/*for (char i = 0; i < 2; i++) if (card_h[i] < 14 && Dflash[card_h[i] % 13] == 0)
						Hcard[count + i] = card_h[i] % 13;*/
					if (card_h[0] < 14 && Dflash[card_h[0] % 13] == 0)
						Hcard[count] = card_h[0] % 13;
					if (card_h[1] < 14 && Dflash[card_h[1] % 13] == 0)
						Hcard[count + 1] = card_h[1] % 13;

				}
				if (CFflag == 1) {
					for (char i = 0; i < 13; i++) if (Cflash[i] == 1) {
						Ccard[count] = i;
						Hcard[count] = i;
						++count;
					}
					/*for (char i = 0; i < 2; i++) if (card_c[i] > 13 && card_c[i] < 27 && Cflash[card_c[i] % 13] == 0)
						Ccard[count + i] = card_c[i] % 13;*/
					if (card_c[0] > 13 && card_c[0] < 27 && Cflash[card_c[0] % 13] == 0) Ccard[count] = card_c[0] % 13;
					if (card_c[1] > 13 && card_c[1] < 27 && Cflash[card_c[1] % 13] == 0) Ccard[count + 1] = card_c[1] % 13;

					/*for (char i = 0; i < 2; i++) if (card_h[i] > 13 && card_h[i] < 27 && Cflash[card_h[i] % 13] == 0)
						Hcard[count + i] = card_h[i] % 13;*/
					if (card_h[0] > 13 && card_h[0] < 27 && Cflash[card_h[0] % 13] == 0) Hcard[count] = card_h[0] % 13;
					if (card_h[1] > 13 && card_h[1] < 27 && Cflash[card_h[1] % 13] == 0) Hcard[count + 1] = card_h[1] % 13;
				}
				if (HFflag == 1) {
					for (char i = 0; i < 13; i++) if (Hflash[i] == 1) {
						Ccard[count] = i;
						Hcard[count] = i;
						++count;
					}
					/*for (char i = 0; i < 2; i++) if (card_c[i] > 26 && card_c[i] < 40 && Hflash[card_c[i] % 13] == 0)
						Ccard[count + i] = card_c[i] % 13;*/
					if (card_c[0] > 26 && card_c[0] < 40 && Hflash[card_c[0] % 13] == 0) Ccard[count] = card_c[0] % 13;
					if (card_c[1] > 26 && card_c[1] < 40 && Hflash[card_c[1] % 13] == 0) Ccard[count + 1] = card_c[1] % 13;

					/*for (char i = 0; i < 2; i++) if (card_h[i] > 26 && card_h[i] < 40 && Hflash[card_h[i] % 13] == 0)
						Hcard[count + i] = card_h[i] % 13;*/
					if (card_h[0] > 26 && card_h[0] < 40 && Hflash[card_h[0] % 13] == 0) Hcard[count] = card_h[0] % 13;
					if (card_h[1] > 26 && card_h[1] < 40 && Hflash[card_h[1] % 13] == 0) Hcard[count + 1] = card_h[1] % 13;
				}
				if (SFflag == 1) {
					for (char i = 0; i < 13; i++) if (Sflash[i] == 1) {
						Ccard[count] = i;
						Hcard[count] = i;
						++count;
					}
					/*for (char i = 0; i < 2; i++) if (card_c[i] > 39 && card_c[i] < 53 && Sflash[card_c[i] % 13] == 0)
						Ccard[count + i] = card_c[i] % 13;*/
					if (card_c[0] > 39 && card_c[0] < 53 && Sflash[card_c[0] % 13] == 0) Ccard[count] = card_c[0] % 13;
					if (card_c[1] > 39 && card_c[1] < 53 && Sflash[card_c[1] % 13] == 0) Ccard[count + 1] = card_c[1] % 13;

					/*for (char i = 0; i < 2; i++) if (card_h[i] > 39 && card_h[i] < 53 && Sflash[card_h[i] % 13] == 0)
						Hcard[count + i] = card_h[i] % 13;*/
					if (card_h[0] > 39 && card_h[0] < 53 && Sflash[card_h[0] % 13] == 0) Hcard[count] = card_h[0] % 13;
					if (card_h[1] > 39 && card_h[1] < 53 && Sflash[card_h[1] % 13] == 0) Hcard[count + 1] = card_h[1] % 13;
				}
				for (char i = 0; i < 7; i++) {
					if (Ccard[i] == 0) Ccard[i] = 13;
					if (Hcard[i] == 0) Hcard[i] = 13;
				}
				qsortRecursive(Ccard, 7);
				qsortRecursive(Hcard, 7);
				for (char i = 6; i > 1; i--) {
					if (Ccard[i] > Hcard[i]) {
						bank_c += bank_all;
						return 1;
					}
					if (Ccard[i] < Hcard[i]) {
						bank_h += bank_all;
						return 2;
					}
					if (Ccard[i] == Hcard[i] && i == 2) {
						bank_c += bank_all >> 1;
						bank_h += bank_all >> 1;
						return 3;
					}
				}
			}
		}
		if (winC == 5) {
			if (HCCombination[5] == 0) HCCombination[5] = 13;
			if (HHCombination[5] == 0) HHCombination[5] = 13;
			if (HCCombination[5] > HHCombination[5]) {
				bank_c += bank_all;
				return 1;
			}
			if (HCCombination[5] < HHCombination[5]) {
				bank_h += bank_all;
				return 2;
			}
			if (HCCombination[5] == HHCombination[5]) {
				if (HCCombination[5] == 13) HCCombination[5] = 0;
				if (HHCombination[5] == 13) HHCombination[5] = 0;
				if (HCCombination[9] > HCCombination[0]) Ccard[0] = HCCombination[9];
				else Ccard[0] = HCCombination[0];
				if (HHCombination[9] > HHCombination[0]) Hcard[0] = HHCombination[9];
				else Hcard[0] = HHCombination[0];
				if (Ccard[0] > Hcard[0]) {
					bank_c += bank_all;
					return 1;
				}
				if (Ccard[0] < Hcard[0]) {
					bank_h += bank_all;
					return 2;
				}
				if (Ccard[0] == Hcard[0]) {
					bank_c += bank_all >> 1;
					bank_h += bank_all >> 1;
					return 3;
				}
			}
		}
		if (winC == 6) {
			if (HCCombination[6] == 0) HCCombination[6] = 13;
			if (HHCombination[6] == 0) HHCombination[6] = 13;
			if (HCCombination[6] > HHCombination[6]) {
				bank_c += bank_all;
				return 1;
			}
			if (HCCombination[6] < HHCombination[6]) {
				bank_h += bank_all;
				return 2;
			}
			if (HCCombination[6] == HHCombination[6]) {
				if (HCCombination[6] == 13) HCCombination[6] = 0;
				if (HHCombination[6] == 13) HHCombination[6] = 0;
				/*for (char i = 0; i < 3; i++) {
					if (flop[i] % 13 == 0 && HCCombination[6] != 0) {
						bank_c += bank_all >> 1;
						bank_h += bank_all >> 1;
						return 3;
					}
					if (flop[i] % 13 != HCCombination[6] && flop[i] % 13 > Ccard[0])
						Ccard[0] = flop[i] % 13;
				}*/
				if ((flop[0] % 13 == 0 && HCCombination[6] != 0) || (flop[1] % 13 == 0 && HCCombination[6] != 0) || (flop[2] % 13 == 0 && HCCombination[6] != 0)) {
					bank_c += bank_all >> 1;
					bank_h += bank_all >> 1;
					return 3;
				}
				if (flop[0] % 13 != HCCombination[6] && flop[0] % 13 > Ccard[0]) Ccard[0] = flop[0] % 13;
				if (flop[1] % 13 != HCCombination[6] && flop[1] % 13 > Ccard[0]) Ccard[0] = flop[1] % 13;
				if (flop[2] % 13 != HCCombination[6] && flop[2] % 13 > Ccard[0]) Ccard[0] = flop[2] % 13;

				if (turn % 13 == 0 && HCCombination[6] != 0) {
					bank_c += bank_all >> 1;
					bank_h += bank_all >> 1;
					return 3;
				}
				if (turn % 13 != HCCombination[6] && turn % 13 > Ccard[0])
					Ccard[0] = turn % 13;
				if (river % 13 == 0 && HCCombination[6] != 0) {
					bank_c += bank_all >> 1;
					bank_h += bank_all >> 1;
					return 3;
				}
				if (river % 13 != HCCombination[6] && river % 13 > Ccard[0])
					Ccard[0] = river % 13;
				Hcard[0] = Ccard[0];
				/*for (char i = 0; i < 2; i++) {
					if (card_c[i] % 13 == 0 && HCCombination[6] != 0) Ccard[0] = 13;
					if (card_h[i] % 13 == 0 && HCCombination[6] != 0) Hcard[0] = 13;
					if (card_c[i] % 13 != HCCombination[6] && card_c[i] % 13 > Ccard[0]) Ccard[0] = card_c[i] % 13;
					if (card_h[i] % 13 != HCCombination[6] && card_h[i] % 13 > Hcard[0]) Hcard[0] = card_h[i] % 13;
				}*/

				if ((card_c[0] % 13 == 0 && HCCombination[6] != 0) || (card_c[1] % 13 == 0 && HCCombination[6] != 0)) Ccard[0] = 13;
				if ((card_h[0] % 13 == 0 && HCCombination[6] != 0) || (card_h[1] % 13 == 0 && HCCombination[6] != 0)) Hcard[0] = 13;

				if (card_c[0] % 13 != HCCombination[6] && card_c[0] % 13 > Ccard[0]) Ccard[0] = card_c[0] % 13;
				if (card_c[1] % 13 != HCCombination[6] && card_c[1] % 13 > Ccard[0]) Ccard[0] = card_c[1] % 13;

				if (card_h[0] % 13 != HCCombination[6] && card_h[0] % 13 > Hcard[0]) Hcard[0] = card_h[0] % 13;
				if (card_h[1] % 13 != HCCombination[6] && card_h[1] % 13 > Hcard[0]) Hcard[0] = card_h[1] % 13;

				if (Ccard[0] > Hcard[0]) {
					bank_c += bank_all;
					return 1;
				}
				if (Ccard[0] < Hcard[0]) {
					bank_h += bank_all;
					return 2;
				}
				if (Ccard[0] == Hcard[0]) {
					bank_c += bank_all >> 1;
					bank_h += bank_all >> 1;
					return 3;
				}
			}
		}
		if (winC == 7) {
			if (HCCombination[7] > HHCombination[7]) {
				bank_c += bank_all;
				return 1;
			}
			if (HCCombination[7] < HHCombination[7]) {
				bank_h += bank_all;
				return 2;
			}
			if (HCCombination[7] == HHCombination[7]) {
				bank_c += bank_all >> 1;
				bank_h += bank_all >> 1;
				return 3;
			}
		}
		if (winC == 8) {
			bank_c += bank_all >> 1;
			bank_h += bank_all >> 1;
			return 3;
		}
	}
}

void contex(RenderWindow& window, char flag) {
	while (1) {
		update(window, flag);
		if (flag == 5) {
			window.draw(winner);
			window.draw(loser);
		}
		if (IntRect(1272, 770, 300, 50).contains(Mouse::getPosition(window))) {
			cont.setColor(Color::Blue);
			window.draw(cont);
		}
		else {
			cont.setColor(Color::White);
			window.draw(cont);
		}
		if (IntRect(1272, 910, 150, 50).contains(Mouse::getPosition(window))) {
			ex.setColor(Color::Blue);
			window.draw(ex);
		}
		else {
			ex.setColor(Color::White);
			window.draw(ex);
		}
		if (IntRect(1272, 770, 300, 50).contains(Mouse::getPosition(window)) && Mouse::isButtonPressed(Mouse::Left)) {
			cont.setColor(Color::Red);
			window.draw(cont);
			window.display();
			break;
		}
		if (IntRect(1272, 910, 150, 50).contains(Mouse::getPosition(window)) && Mouse::isButtonPressed(Mouse::Left)) {
			ex.setColor(Color::Red);
			window.draw(ex);
			window.display();
			window.close();
			exit(1);
		}
		window.display();
	}


}

int main() {

	short x = 0, y = 0;
	char flag = 0;
	dealer_chip = 1;
	srand(time(NULL));
	RenderWindow window(VideoMode(1920, 1080), "Pocker");

	menu(window);
	//LVL

	fieldtexture.loadFromFile("images/field.png");
	cardstexture.loadFromFile("images/cards.png");

	field.setTexture(fieldtexture);

	font.loadFromFile("AndadaPro-Regular.ttf");
	bank.setColor(Color::White); bank_hum.setColor(Color::White); bank_com.setColor(Color::White);
	bank.setPosition(1272, 133); bank_hum.setPosition(460, 929); bank_com.setPosition(460, 100); //Банки

	hrate.setColor(Color::Red);
	hrate.setPosition(1400, 768); //Да-да, это двое точие, когда делаем ставки.

	rate_cchip.setColor(Color::Black); rate_hchip.setColor(Color::Black);
	rate_cchip.setPosition(530, 232); rate_hchip.setPosition(530, 802); //Фишка-ставки.

	winner.setColor(Color::Magenta);
	loser.setColor(Color::Magenta);
	loser.setString("LOSER!");
	winner.setString("WINNER!");

	cards.setTexture(cardstexture); //стопка карт.
	cards.setTextureRect(IntRect(295, 857, 148, 215));
	cards.setPosition(200, 433);

	cardh1.setTexture(cardstexture); //1ая карта чела
	cardh1.setPosition(787, 765);

	cardh2.setTexture(cardstexture); //2ая карта чела.
	cardh2.setPosition(985, 765);

	cardc1.setTexture(cardstexture); //1я карта компа
	cardc1.setPosition(787, 100);

	cardc2.setTexture(cardstexture); //2ая карта компа
	cardc2.setPosition(985, 100);

	card1.setTexture(cardstexture); //1й флоп
	card1.setPosition(490, 433);

	card2.setTexture(cardstexture); //2ой флоп
	card2.setPosition(678, 433);

	card3.setTexture(cardstexture); //3ий флоп
	card3.setPosition(876, 433);

	card4.setTexture(cardstexture); //терн
	card4.setPosition(1074, 433);

	card5.setTexture(cardstexture); //ривер
	card5.setPosition(1272, 433);

	chiptexture.loadFromFile("images/fishka.png"); //фишка диллера
	cchip.setTexture(chiptexture);
	cchip.setPosition(460, 160);
	hchip.setTexture(chiptexture);
	hchip.setPosition(460, 727);
	dealertexture.loadFromFile("images/dealer.png");
	dealer.setTexture(dealertexture);

	passtexture.loadFromFile("images/pass.png");
	ratetexture.loadFromFile("images/rate.png");
	calltexture.loadFromFile("images/call.png");
	checktexture.loadFromFile("images/check.png");
	oktexture.loadFromFile("images/ok.png");
	extexture.loadFromFile("images/exit.png");
	conttexture.loadFromFile("images/continue.png");

	pass.setTexture(passtexture);
	check.setTexture(checktexture);
	call.setTexture(calltexture);
	rate.setTexture(ratetexture);
	cpass.setTexture(passtexture);
	ccheck.setTexture(checktexture);
	ccall.setTexture(calltexture);
	crate.setTexture(ratetexture);
	ok.setTexture(oktexture);
	ex.setTexture(extexture);
	cont.setTexture(conttexture);

	rate.setPosition(1272, 770);
	call.setPosition(1272, 840);
	pass.setPosition(1272, 910);
	check.setPosition(1272, 840);
	crate.setPosition(885, 320);
	ccall.setPosition(885, 320);
	cpass.setPosition(885, 320);
	ccheck.setPosition(885, 320);
	ok.setPosition(1475, 770);
	ex.setPosition(1272, 910);
	cont.setPosition(1272, 770);


	rate.setColor(Color::White);
	call.setColor(Color::White);
	pass.setColor(Color::White);
	check.setColor(Color::White);
	crate.setColor(Color::Red);
	ccall.setColor(Color::Red);
	cpass.setColor(Color::Red);
	ccheck.setColor(Color::Red);
	ok.setColor(Color::White);

	while (window.isOpen()) {

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		if (dealer_chip == 0) dealer.setPosition(1140, 100);
		else dealer.setPosition(1140, 908);

		BEGIN();

		RATES_BEGIN();

		START_HAND();
		/*for (char i = 0; i < 2; i++) {
			if (i == 0) {
				check_cards(card_h[i], &x, &y);
				cardh1.setTextureRect(IntRect(x, y, 148, 215));
			}
			if (i == 1) {
				check_cards(card_h[i], &x, &y);
				cardh2.setTextureRect(IntRect(x, y, 148, 215));
			}
		}*/
		check_cards(card_h[0], &x, &y);
		cardh1.setTextureRect(IntRect(x, y, 148, 215));

		check_cards(card_h[1], &x, &y);
		cardh2.setTextureRect(IntRect(x, y, 148, 215));

		cardc1.setTextureRect(IntRect(295, 857, 148, 215));
		cardc2.setTextureRect(IntRect(295, 857, 148, 215));

		update(window, 1);
		window.display();

		CHECK_STARTHAND();

		if (bank_h != 0 && bank_c != 0) {
			RATES(window, 1);
			if (end_game == 1 || end_game == 2) {
				ENDGAME();
				contex(window, 1);
				continue;
			}
		}

		FLOP();
		/*for (char i = 0; i < 3; i++) {
			if (i == 0) {
				check_cards(flop[i], &x, &y);
				card1.setTextureRect(IntRect(x, y, 148, 215));
			}
			if (i == 1) {
				check_cards(flop[i], &x, &y);
				card2.setTextureRect(IntRect(x, y, 148, 215));
			}
			if (i == 2) {
				check_cards(flop[i], &x, &y);
				card3.setTextureRect(IntRect(x, y, 148, 215));
			}
		}*/

		check_cards(flop[0], &x, &y);
		card1.setTextureRect(IntRect(x, y, 148, 215));
		check_cards(flop[1], &x, &y);
		card2.setTextureRect(IntRect(x, y, 148, 215));
		check_cards(flop[2], &x, &y);
		card3.setTextureRect(IntRect(x, y, 148, 215));

		update(window, 2);
		window.display();

		PAIR_AND_SET_TABLE(turn_flag, river_flag);
		STRAIGHT();
		FLASH();
		CHECK_STRAIGHTDRO();
		CHECK_FLUSHDRO();
		PROBABILITY();
		CHECK_TABLE();
		CHECK_HAND(HCCombination, card_c);
		//CHECK_HAND(HHCombination, card_h);

		if (bank_h != 0 && bank_c != 0) {
			RATES(window, 2);
			if (end_game == 1 || end_game == 2) {
				ENDGAME();
				contex(window, 2);
				continue;
			}
		}

		TURN();
		check_cards(turn, &x, &y);
		card4.setTextureRect(IntRect(x, y, 148, 215));

		update(window, 3);
		window.display();

		PAIR_AND_SET_TABLE(turn_flag, river_flag);
		STRAIGHT();
		FLASH();
		CHECK_STRAIGHTDRO();
		CHECK_FLUSHDRO();
		PROBABILITY();
		CHECK_TABLE();
		CHECK_HAND(HCCombination, card_c);
		//CHECK_HAND(HHCombination, card_h);

		if (bank_h != 0 && bank_c != 0) {
			RATES(window, 3);
			if (end_game == 1 || end_game == 2) {
				ENDGAME();
				contex(window, 3);
				continue;
			}
		}

		RIVER();
		check_cards(river, &x, &y);
		card5.setTextureRect(IntRect(x, y, 148, 215));

		update(window, 4);
		window.display();

		PAIR_AND_SET_TABLE(turn_flag, river_flag);
		STRAIGHT();
		FLASH();
		CHECK_TABLE();
		CHECK_HAND(HCCombination, card_c);
		CHECK_HAND(HHCombination, card_h);

		if (bank_h != 0 && bank_c != 0) {
			RATES(window, 4);
			if (end_game == 1 || end_game == 2) {
				ENDGAME();
				contex(window, 4);
				continue;
			}
		}

		/*for (char i = 0; i < 2; i++) {
			if (i == 0) {
				check_cards(card_c[i], &x, &y);
				cardc1.setTextureRect(IntRect(x, y, 148, 215));
			}
			if (i == 1) {
				check_cards(card_c[i], &x, &y);
				cardc2.setTextureRect(IntRect(x, y, 148, 215));
			}
		}*/

		check_cards(card_c[0], &x, &y);
		cardc1.setTextureRect(IntRect(x, y, 148, 215));

		check_cards(card_c[1], &x, &y);
		cardc2.setTextureRect(IntRect(x, y, 148, 215));

		check_cards(card_c[0], &x, &y);
		cardc1.setTextureRect(IntRect(x, y, 148, 215));
		check_cards(card_c[1], &x, &y);
		cardc2.setTextureRect(IntRect(x, y, 148, 215));


		flag = WINNER(window);
		bank_all = 0;
		if (flag == 1) {
			winner.setPosition(885, 330);
			loser.setPosition(885, 700);

		}
		if (flag == 2) {
			winner.setPosition(885, 700);
			loser.setPosition(885, 330);
		}
		if (flag == 3) {
			winner.setPosition(885, 700);
			loser.setString("WINNER!");
			loser.setPosition(885, 330);
		}

		contex(window, 5);
		_sleep(500);
		if (flag == 3) loser.setString("LOSER!");

		if (dealer_chip == 0) dealer_chip = 1;
		else dealer_chip = 0;

		if (bank_c < 50 || bank_h < 50) {
			bank_c = 1000; bank_h = 1000;
			window.clear();
			menu(window);
		}
	}

	return 0;
}