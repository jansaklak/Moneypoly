#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX 1000

FILE *write_names, *write_money, *write_positions, *write_ownerid, *write_buildinglevel,*open_names, *open_positions, *open_money, *open_ownerid, *open_buildinglevel, *set_in, *set_rounds, *write_taxincome;

char name_input[50];
int counter,loop,rounds;

void entername(char *name_input)
{
	printf("Wprowadz imie\n\t");
	scanf(" %s",name_input);
}

int dice()
{
    srand(time(NULL));
    int num = (rand() % (6 - 1 + 1)) + 1;
    num = (rand() % (6 - 1 + 1)) + 1;
    num = (rand() % (6 - 1 + 1)) + 1;
    num = (rand() % (6 - 1 + 1)) + 1;
    num = (rand() % (6 - 1 + 1)) + 1;
    num = (rand() % (6 - 1 + 1)) + 1;
    return num;
}

void write_setup(int ust)
{
	set_in = fopen("setup.txt","w");

	if(!set_in)
	{
		printf("Błąd otwarcia pliku do zapisu danych\n");
		exit(-1);
	}

	fprintf(set_in,"%d",ust);
	fclose(set_in);
}

struct player
{
    char name[50];
    double money;
    int position;
    double tax_income;
};

struct player players[MAX];

struct district
{
    char name[50];
    char building_name[50];
    double building_price;
    double building_income;
    int building_level;
    int ownerID;
    double price;
    double tax;
};

struct district districts[27];

char menu()
{
    char m;
    printf("\nWitaj w grze\n");
    printf("\nNacisnij N żeby rozpocząć nową gre\n");
    printf("Nacisnij W żeby wczytać ostatnią gre\n\t");
    scanf("%s",&m);
    printf("\n");
    return m;
}


char plot_buy()
{
    char b;
    printf("Nacisnij T żeby kupić\n");
    printf("Naciśnij N żeby nie kupować\n\t");
    scanf("%s",&b);
    printf("\n");
    return b;

}

char building_buy()
{
    char b;
    printf("Nacisnij T żeby kupić budynek\n");
    printf("Naciśnij N żeby nie kupować\n\t");
    scanf("%s",&b);
    printf("\n");
    return b;
}


int main()
{
    printf("uruchomiono gre\n");

    loop = 1;
    while(loop == 1)
    switch (menu())
	{
        default:

		    printf("\t\tNie wybrano opcji\n");

        case 'N': /*/ Nowa gra /*/

            printf("Rozpoczęto nową gre\nPodaj liczbe graczy\n\t");
            scanf("%d",&counter);
			for(int i = 0;i<counter;i++)
			{
				entername(name_input);
				strcpy(players[i].name,name_input);
				players[i].money = 1000;
                players[i].tax_income = 0;
			}
            write_names = fopen("names.txt","w");
	        write_money = fopen("money.txt","w");
            write_positions = fopen("positions.txt","w");
            write_buildinglevel = fopen("buildinglevel.txt","w");
            write_ownerid = fopen("ownerid.txt","w");
            write_taxincome = fopen("taxincome.txt","w");

            if(!write_money|!write_names|!write_positions|!write_buildinglevel|!write_ownerid|!write_taxincome)
			{
				printf("Błąd otwarcia plików do zapisu\n");
				exit(-1);
			}

            for(int k = 0;k<counter;k++)
			{
				fprintf(write_names,"%s\n",players[k].name);
				fprintf(write_money,"%lg\n",players[k].money);
                fprintf(write_positions,"%d\n",players[k].position);
			}

            for(int k = 0;k<28;k++)
            {
                districts[k].ownerID = -1;
                fprintf(write_buildinglevel,"%d\n",districts[k].building_level);
                fprintf(write_ownerid,"%d\n",districts[k].ownerID);
            }

            printf("Podaj liczbe rund\n\t");
            scanf("%d",&rounds);

            for (int dist=0;dist<28;dist++)
            {
                districts[dist].ownerID = -1;  
            }


            fclose(write_positions);
            fclose(write_money);
            fclose(write_names);
            fclose(write_buildinglevel);
            fclose(write_ownerid);
            fclose(write_taxincome);

			write_setup(counter);
            loop = 0;

		break;

        case 'W': /*/Wczytaj gre/*/

            printf("Wczytano gre");
            printf("Podaj liczbe rund\n");
            scanf("%d",&rounds);
            
            open_names = fopen("names.txt","r");
    	    open_money = fopen("money.txt","r");
            open_positions = fopen("positions.txt","r");
            open_buildinglevel = fopen("buildinglevel.txt","r");
            open_ownerid = fopen("ownerid.txt","r");
            

			if(!open_names|!open_money|!open_positions)
			{
				printf("Błąd otwarcia pliku do odczytu\n");
				exit(-1);
			}

			int j = 0;


			while(fscanf(open_money,"%lg",&players[j].money) == 1)
			{
				fscanf(open_names,"%s",players[j].name);
                fscanf(open_positions,"%d",&players[j].position);
				j++;
        		counter = j;
			}

            int d = 0;
            while(d<28)
            {
                fscanf(open_ownerid,"%d",&districts[d].ownerID);
                fscanf(open_buildinglevel,"%d",&districts[d].building_level);
                d++;
            }

			fclose(open_names);
			fclose(open_money);
            fclose(open_positions);
            fclose(open_ownerid);
            fclose(open_buildinglevel);

            loop = 0;
			break;

        
    }
    /*/ Ustaw ceny i nazwy dzielnic/*/
    strcpy(districts[0].name,"Pozycja startowa");
    for (int dist = 1;dist<5;dist++)
    {
        strcpy(districts[dist].name,"Przedmieście");
        districts[dist].price = 300;
        strcpy(districts[dist].building_name,"Dom");
        districts[dist].building_price = 250;
        districts[dist].tax = 50;
    }
    for (int dist = 5;dist<10;dist++)
    {
        strcpy(districts[dist].name,"Miasto");
        districts[dist].price = 500;
        strcpy(districts[dist].building_name,"Blok");
        districts[dist].building_price = 500;
        districts[dist].tax = 75;
    }
    for (int dist = 10;dist<15;dist++)
    {
        strcpy(districts[dist].name,"Strefa biurowa");
        districts[dist].price = 800;
        strcpy(districts[dist].building_name,"Wieżowiec");
        districts[dist].building_price = 1000;
        districts[dist].tax = 100;
    }
    for (int dist = 15;dist<17;dist++)
    {
        strcpy(districts[dist].name,"Stare miasto");
        districts[dist].price = 900;
        strcpy(districts[dist].building_name,"Hotel");
        districts[dist].building_price = 750;
        districts[dist].tax = 150;
    }
    for (int dist = 17;dist<21;dist++)
    {
        strcpy(districts[dist].name,"Przedmieście");
        districts[dist].price = 300;
        strcpy(districts[dist].building_name,"Dom");
        districts[dist].building_price = 250;
        districts[dist].tax = 50;
    }
    for (int dist = 21;dist<28;dist++)
    {
        strcpy(districts[dist].name,"Strefa przemysłowa");
        districts[dist].price = 600;
        strcpy(districts[dist].building_name,"Fabryka");
        districts[dist].building_price = 1000;
        districts[dist].tax = 200;
    }

    /*/ Gra /*/

    for (int game_round_counter = 0; game_round_counter<rounds; game_round_counter++)
    {
        printf("\nRozpoczęto %d z %d kolejek\n", game_round_counter+1,rounds);

        /*/ Zapis gry co runde /*/

        write_names = fopen("names.txt","w");
        write_money = fopen("money.txt","w");
        write_positions = fopen("positions.txt","w");
        write_buildinglevel = fopen("buildinglevel.txt","w");
        write_ownerid = fopen("ownerid.txt","w");

        if(!write_money|!write_names|!write_positions|!write_buildinglevel|!write_ownerid)
			{
				printf("Błąd otwarcia plików do zapisu\n");
				exit(-1);
			}
    
        for(int k = 0;k<counter;k++)
			{
                
				fprintf(write_names,"%s\n",players[k].name);
				fprintf(write_money,"%lg\n",players[k].money);
                fprintf(write_positions,"%d\n",players[k].position);
			}

        for(int k = 0;k<28;k++)
            {
                fprintf(write_buildinglevel,"%d\n",districts[k].building_level);
                fprintf(write_ownerid,"%d\n",districts[k].ownerID);
            }

        fclose(write_names);
		fclose(write_money);
        fclose(write_positions);
        fclose(write_ownerid);
        fclose(write_buildinglevel);

        /*/ Koniec zapisu /*/

        for (int player_counter = 0; player_counter < counter;player_counter++)
        {
            int move;
            int ID;
            char ID_to_name[50];
            int pose;
            int owned;
            move = dice();
            int buying_loop = 0;

            printf("\nKolej %d. gracza: \t%s\t", player_counter+1,players[player_counter].name);
            printf("\tPozycja: %d\tStan konta: %lg\n",players[player_counter].position,players[player_counter].money);
            players[player_counter].position += move;
            if (players[player_counter].position>27)
            {
                players[player_counter].position -= 28;
                printf("Otrzymujesz 500zł za okrążenie\n");
                players[player_counter].money += 500;
            }
            pose = players[player_counter].position;

            ID = districts[pose].ownerID;
            
            if (ID == -1)
            {
                strcpy(ID_to_name,"Nikogo");
                owned = 0;
            }

            else
            {
                strcpy(ID_to_name,players[ID].name);
                owned = 1;
            }


            printf("\nWylosowano %d. Pozycja zmienia się na %d\n\n",move,players[player_counter].position);
            
                if(districts[pose].ownerID == player_counter)
                {
                    printf("\t\t%s\nJesteś na swojej działce\n",players[player_counter].name);
                    buying_loop = 1; 
                    if(players[player_counter].money<=0)
                    {
                        printf("Masz za mało pieniędzy. Omijasz kolejke\n");
                        buying_loop = 0;
                    }
                }

                else
                {
                if(pose==0)
                {
                        printf("Znajdujesz się na starcie, nie możesz nic zrobić\n");
                }

                else
                {

                if(owned == 0)
                    {
                        int plot_loop = 1;
                        if(players[player_counter].money<districts[pose].price)
                        {
                            printf("Masz za mało pieniędzy. Omijasz kolejke\n");
                            buying_loop = 0;
                            plot_loop = 0;
                        }
                        else
                        {
                        printf("\t%s\t\t\t\tMożesz ją kupić za %lg\n\tDziałka nie należy do nikogo\t\tKoszt budynku: %lg\n\t\t\t\t\t\tPodatek dla innych:%lg*liczba budynków\n\n",districts[pose].name,districts[pose].price,districts[pose].building_price,districts[pose].tax);
                        
                        printf("%s, Posiadasz %lg\n",players[player_counter].name,players[player_counter].money);
    
                        while(plot_loop == 1)
                        {   switch(plot_buy())
                            {    
                            default:
                                printf("\tNie wybrano opcji\n");
                            case 'T':
                                if(players[player_counter].money<districts[pose].price)
                                {
                                    printf("\tMasz za mało pieniędzy\n");
                                    plot_loop = 0;
                                    break;
                                }
                                printf("\t\t\t\tKupiono działke\n");
                                players[player_counter].money -= districts[pose].price;
                                districts[pose].ownerID = player_counter;
                                plot_loop = 0;
                                buying_loop = 1;
                                break;
                            case 'N':
                                printf("\t\t\t\tNie kupiono działki\n");
                                plot_loop = 0;
                                break;
                            }
                        }
                        }
                    
                    }

                    else
                    {
                        printf("\t\t\tStoisz na dzialce należącej do %s\n\t\t\tMusisz zapłacić %lg podatku\n",ID_to_name,districts[pose].tax*districts[pose].building_level);
                        players[player_counter].money -= districts[pose].tax*districts[pose].building_level;
                        players[ID].money += districts[pose].tax*districts[pose].building_level;
                        players[ID].tax_income += districts[pose].tax*districts[pose].building_level;
                    }
                }
            }


            while(buying_loop == 1)
            {
                double current_price = districts[pose].building_price*districts[pose].building_level+districts[pose].building_price;
                if(players[player_counter].money<current_price)
                {
                printf("Masz za mało pieniędzy %lg. Omijasz kolejke\n",players[player_counter].money);
                buying_loop = 0;
                }
                else
                {
                printf("Możesz kupić %s za %lg\n",districts[pose].building_name,current_price);

                switch(building_buy())
                {
                default:

                    printf("Nie wybrano opcji\n");

                case 'T':
                
                    if(players[player_counter].money<current_price)
                    {
                        printf("Masz za mało pieniędzy\n");
                        break;
                    }
                    else
                    {    printf("Kupiono %s za %lg\n",districts[pose].building_name,current_price);
                        players[player_counter].money -= current_price;
                        districts[pose].building_level ++;
                        break;
                    }

                case 'N':

                    printf("Nie kupiono nic");
                    buying_loop = 0;
                    break;
                }
                }
            }
            printf("\t\t\t\t\tZostało Ci %lg zł\n",players[player_counter].money);
        }

    }
        printf("\nZakończono gre\n");

        /*/ Zapis gry i koniec /*/

        write_names = fopen("names.txt","w");
        write_money = fopen("money.txt","w");
        write_positions = fopen("positions.txt","w");
        write_buildinglevel = fopen("buildinglevel.txt","w");
        write_ownerid = fopen("ownerid.txt","w");
        write_taxincome = fopen("taxincome.txt","w");

        if(!write_money|!write_names|!write_positions|!write_buildinglevel|!write_ownerid)
			{
				printf("Błąd otwarcia plików do zapisu\n");
				exit(-1);
			}
    
        for(int k = 0;k<counter;k++)
			{
                
				fprintf(write_names,"%s\n",players[k].name);
				fprintf(write_money,"%lg\n",players[k].money);
                fprintf(write_positions,"%d\n",players[k].position);
                fprintf(write_taxincome,"%lg\n",players[k].tax_income);
			}

        for(int k = 0;k<28;k++)
            {
                fprintf(write_buildinglevel,"%d\n",districts[k].building_level);
                fprintf(write_ownerid,"%d\n",districts[k].ownerID);
            }

        fclose(write_names);
		fclose(write_money);
        fclose(write_positions);
        fclose(write_ownerid);
        fclose(write_buildinglevel);
        fclose(write_taxincome);
        system("pause");
}