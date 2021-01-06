#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <signal.h>

pthread_mutex_t locksum, lockcreator;

int summ; //matrix elemanlarının sayılarının toplamını ifade ediyor.
int adet; //kaç adet matrix oluşturulacağını tutuyoruz.
int matrix[20][20];

void cntl_z_handler(int dummy) //ctrl+z ye basılınca program kapatlıyor.
{
	signal(SIGTSTP, cntl_z_handler);
	printf("program kapatılıyor.. \n");
	exit(1);
}

int getRandoms() //0-100 arası bir adet random sayı üretme fonksiyonu
{
	int i;
	for (i = 0; i < 1; i++)
	{
		int num = (rand() % 101);
		return num;
	}
}

void *sum() // creator() tarafından oluşturulan matrixin içindeki elemanların toplamını döndürür.
{

	for (int n = 0; n < adet; n++)
	{

		pthread_mutex_lock(&locksum);

		for (int i = 0; i < 20; i++)
		{
			for (int j = 0; j < 20; j++)
			{
				summ += matrix[i][j];
				//printf("summ = %d \n", summ);
			}
		}
		printf("%d. matrixin elemanları toplamı: %d \n", n + 1, summ);
		printf("\n");
		summ = 0; //her toplamdan sonra toplam sayacını sıfırlıyoruz.
		pthread_mutex_unlock(&lockcreator);
	}
}

void *creator() //girilen adet sayısı kadar rastgele elemanlardan oluşan 20x20 matrix oluşturur.
{

	for (int n = 0; n < adet; n++)
	{

		pthread_mutex_lock(&lockcreator);
		printf("%d. matrix :\n", (n + 1));
		for (int i = 0; i < 20; i++)
		{
			for (int j = 0; j < 20; j++)
			{

				int rnd = getRandoms();
				matrix[i][j] = rnd;
				printf("%d ", rnd);
			}
			printf("\n");
		}

		printf("\n");
		pthread_mutex_unlock(&locksum);
	}
}

int main()
{

	signal(SIGTSTP, cntl_z_handler);
	while (1)
	{
		pthread_mutex_init(&locksum, NULL);
		pthread_mutex_init(&lockcreator, NULL);
		pthread_mutex_lock(&locksum);

		pthread_t sumthread, creatorthread;
		adet=0; //bu olmadığında ikinci defa matrix adedi sorulduğunda string girerse sonsuz döngüye girer.
		printf("Oluşturulucak matrix adedini giriniz: ");
		scanf("%d", &adet);
	
		if ( adet < 50 && adet>0 )
		{
			summ = 0;

			void *status;

			pthread_create(&creatorthread, NULL, creator, NULL);
			pthread_create(&sumthread, NULL, sum, NULL);

			pthread_join(creatorthread, &status);
			pthread_join(sumthread, &status);
		}
		else if (adet <= 0) //0 veya daha küçük değer girildiğinde program kapatlıyor.
		{
			printf("String, 0 veya daha küçük sayı girildi. Program kapatılıyor.. \n");
			exit(1);
		}
		else
		{
			printf("50 den küçük sayi girmeniz gerekmektedir. Program kapatılıyor.. \n");
			exit(1);
		}
	}

	return 0;
}
