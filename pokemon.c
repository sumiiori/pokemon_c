#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

typedef struct{
	char name[100];//ポケモンの名前
	int H; //ポケモンの体力
	double A; //攻撃力
	double B; //防御力
	int S; //素早さ
	char skill1[100];//技１
	char skill2[100];//技２
	float skill_effect;//技1の効果の分岐
}POKEMON;

void menu(POKEMON a, POKEMON b);//戦闘画面のメニュー
void myturn(POKEMON *a, POKEMON *b, int skill);//自分の行動
void yourturn(POKEMON *a, POKEMON *b);//相手の行動
double damage(POKEMON attack, POKEMON bougyo);//ダメージ計算

int main(void){
	int kotaiti[12];//ポケモンの個体値
	srand(time(NULL));
	for(int i = 0; i < 12; i++){
		kotaiti[i] = rand() % 6;
	}
	POKEMON hitokage = {"ヒトカゲ", 20 + kotaiti[0], 13 + kotaiti[1], 10 + kotaiti[2], 13 + kotaiti[3], "にらみつける", "ひっかく", 1};
	POKEMON husigidane = {"フシギダネ", 21 + kotaiti[4], 12 + kotaiti[5], 12 + kotaiti[6], 11 + kotaiti[7], "なきごえ", "たいあたり", 0};
	POKEMON zenigame = {"ゼニガメ", 20 + kotaiti[8], 11 + kotaiti[9], 13 + kotaiti[10], 10 + kotaiti[11], "しっぽをふる", "たいあたり", 1};
	POKEMON serect;//自分で選ぶポケモン
	POKEMON raival;//ライバルが選ぶポケモン
	int serect_number;//選ぶ組み合わせ
	int ans = 0;//質問の返答

	while(ans != 1){//ポケモンを選ぶまで繰り返す
		printf("オーキド「３びきの　なかから　ポケモンを　えらぶのじゃ」\n");
		sleep(1);
		printf("1:%s, 2:%s, 3:%s\n", hitokage.name, husigidane.name, zenigame.name);
		scanf("%d", &serect_number);
		if(serect_number == 1){
			serect = hitokage;
			raival = zenigame;
		}else if(serect_number == 2){
			serect = husigidane;
			raival = hitokage;
		}else{
			serect = zenigame;
			raival = husigidane;
		}
		printf("-------------------------------------------\n");
		printf("オーキド「%sで　いいのじゃな？」\n", serect.name);
		printf("1:はい,　2:いいえ\n");
		scanf("%d", &ans);
	}

	printf("-------------------------------------------\n");
	printf("あなたは　オーキドから　%sを　もらった！\n", serect.name);
	printf("ライバル「じゃ　おれは　これ！」\n");
	printf("ライバルは　オーキドから　%sを　もらった！\n", raival.name);
	printf("ライバル「せっかく　じーさんに　ポケモン　もらったんだぜ！\n");
	printf("         …ちょっと　おれの　あいて　してみろ！」\n");
	printf("-------------------------------------------\n");
	printf("ライバルが　しょうぶを　しかけてきた！\n");
	printf("ライバルは　%sを　くりだした！\n", raival.name);
	printf("ゆけっ！　%s！\n", serect.name);

	
	menu(serect, raival);

	return 0;
}

void menu(POKEMON a, POKEMON b){
	int act1,act2;
	int base_aH = a.H;
	int base_bH = b.H;
	while(a.H > 0 && b.H >0){
		printf("%s\nHP:%d/%d\n", a.name, a.H, base_aH);
		printf("%s\nHP:%d/%d\n", b.name, b.H, base_bH);
		printf("-------------------------------------------\n");
		printf("1:たたかう, 2:もちもの, 3:にげる\n");
		scanf("%d", &act1);
		printf("-------------------------------------------\n");
		if(act1 == 1){
			printf("1:%s, 2:%s\n", a.skill1, a.skill2);
			scanf("%d", &act2);
			printf("-------------------------------------------\n");
			if(a.S >= b.S){
				myturn(&a, &b, act2);
				if(b.H <= 0){
					printf("あいての %sは たおれた\n", b.name);
					break;
				}
				yourturn(&b, &a);
				if(a.H <= 0){
					printf("%sは たおれた\n", a.name);
				}
			}else{
				yourturn(&b, &a);
				if(a.H <= 0){
					printf("%sは たおれた\n", a.name);
					break;
				}
				myturn(&a, &b, act2);
				if(b.H <= 0){
					printf("あいての %sは たおれた\n", b.name);
				}
			}
			
		}else if(act1 == 2){
			printf("1:きずぐすり、2:モンスターボール\n");
			scanf("%d",&act2);
			if(act2 == 1){
				a.H = base_aH;
				printf("%sの HPが　かいふくした！\n",a.name);
				printf("-------------------------------------------\n");
				yourturn(&b, &a);
				if(a.H <= 0){
					printf("%sは たおれた\n", a.name);
				}
			}else{
				printf("ひとの　ものを　とったら　どろぼう！\n");
			}
		}else{
			printf("にげられないよ！\n");
		}
	}
	
	printf("-------------------------------------------\n");

	if(a.H <= 0){
		printf("ライバルとの　しょうぶに　まけた\n");
		printf("ライバル「まだまだだな」\n");
	}else if(b.H <= 0){
		printf("ライバルとの　しょうぶに　かった！\n");
		printf("ライバル「えー　そんな　ばかな！\n");
		printf("         おまえの　ポケモンに　すりゃあ　よかった！」\n");
	}
	printf("-------------------------------------------\n");
}

void myturn(POKEMON *a, POKEMON *b, int skill){
	if(skill == 1){
		printf("%sの　%s　こうげき！\n", a->name, a->skill1);
		if(a->skill_effect == 0){
			b->A = b->A * (2 / 3);
			printf("てきの%sの　こうげきりょくが　さがった\n", b->name);
		}else{
			b->B = b->B * (3 / 2);
			printf("てきの%sの　ぼうぎょりょくが　さがった\n", b->name);
		}
	}else{
		printf("%sの　%s！\n", a->name, a->skill2);
		int dame = damage(*a, *b);
		printf("てきの　%sに　%dのダメージ！\n", b->name, dame);
		b->H = b->H - dame;
	}
	printf("-------------------------------------------\n");

}

void yourturn(POKEMON *a, POKEMON *b){
	srand(time(NULL));
	double patarn = rand() % 4;//敵の動きをランダムに
	if(patarn <= 1){
		printf("てきの　%sの　%s　こうげき！\n", a->name, a->skill1);
		if(a->skill_effect == 0){
			b->A = b->A - 1;
			printf("%sの　こうげきりょくが　さがった\n", b->name);
		}else{
			b->B = b->B - 1;
			printf("%sの　ぼうぎょりょくが　さがった\n", b->name);
		}
	}else{
		printf("てきの　%sの　%s！\n", a->name, a->skill2);
		int dame = damage(*a, *b);
		printf("%sに　%dのダメージ！\n", b->name, dame);
		b->H = b->H - dame;
	}
	printf("-------------------------------------------\n");

}


double damage(POKEMON attack, POKEMON bougyo){
	float dame;
	srand(time(NULL));
	double ransu = rand() % 16 + 85;
	int critical = rand() % 16;

	dame = (40 * attack.A / bougyo.B / 50 + 2) * ransu / 100 + 2;
	if(critical == 0){
		dame = dame * 2;
		printf("きゅうしょに　あたった！\n");
	}

	return dame;
}

