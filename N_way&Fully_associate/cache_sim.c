#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#define MAX 300000

char firstLine[200], cmdLine[200];
int mode, cache_size, block_size, lru, address, data;
int miss_count, count, block_num, timer[MAX][128];
double start, end;

struct
{
	unsigned int index;
	unsigned int tag;
	unsigned int data;
} cache[MAX][128];


int main(int argc, char **argv)
{
	int location_bit, index, tag, temp, tag_bit, i, chk, mode_bit, max_chk, ccnt;

	if (argc != 6)
	{
		fputs("Input Error!\n", stderr);
		exit(1);
	}
	mode = atoi(argv[1]);
	cache_size = atoi(argv[2]);
	block_size = atoi(argv[3]);
	lru = atoi(argv[5]);

	temp = cache_size * 1024 / block_size;
	ccnt = temp;
	tag_bit = 0;
	location_bit = 1;
	mode_bit = 0;
	while (temp != 1)
	{
		temp = temp >> 1;
		tag_bit++;
		location_bit = location_bit << 1;
	}
	temp = block_size;
	while (temp != 1)
	{
		temp = temp >> 1;
		block_num++;
	}
	temp = mode;
	while (temp > 1)
	{
		temp = temp >> 1;
		mode_bit++;
		location_bit = location_bit >> 1;
		tag_bit--;
	}
	location_bit--;

	FILE *in = fopen(argv[4], "r");
	if (in == NULL)
	{
		fputs("file error!", stderr);
		exit(1);
	}
	start = clock();
	fgets(firstLine, 200, in);

	while (fgets(cmdLine, 200, in))
	{
		i = 0; address = 0; data = 0; chk = 0;
		while (chk<2)
		{
			if (cmdLine[i] == ' ')
				chk++;
			i++;
		}
		while (1)
		{
			if (cmdLine[i] - '0' >= 0 && cmdLine[i] - '0' <= 9)
				address = address * 10 + (cmdLine[i] - '0');
			else
				break;
			i++;
		}
		i++;
		while (1)
		{
			if (cmdLine[i] - '0' >= 0 && cmdLine[i] - '0' <= 9)
				data = data * 10 + (cmdLine[i] - '0');
			else
				break;
			i++;
		}
		count++;
		//printf("%d %d %d\n", address, data, count);
		if (mode == 0)
		{
			tag = address >> block_num;
			index = tag&location_bit;
			tag = tag >> tag_bit;
			if (cache[index][0].tag != tag)
			{
				miss_count++;
				cache[index][0].tag = tag;
				cache[index][0].data = data;
			}
			else if (cache[index][0].tag == tag && cache[index][0].data != data)
			{
				miss_count++;
				cache[index][0].data = data;
			}
		}
		else if (mode == -1)
		{

			tag = address >> block_num;
			for (i = 0; i<ccnt; i++)
			{
				if (cache[i][0].tag == tag)
				{
					if (cache[i][0].data == data) break;
					else
					{
						miss_count++;
						cache[i][0].tag = tag;
						cache[i][0].data = data;
						timer[i][0] = count;
						break;
					}
				}
				if (cache[i][0].tag == 0)
				{
					miss_count++;
					cache[i][0].tag = tag;
					cache[i][0].data = data;
					timer[i][0] = count;
					break;
				}
				if (i == 0) max_chk = i;
				if (lru == 1)
				{
					if (timer[max_chk][0] > timer[i][0]) max_chk = i;
				}
				else
				{
					if (timer[max_chk][0] < timer[i][0]) max_chk = i;
				}
			}
			if (i == ccnt)
			{
				miss_count++;
				cache[max_chk][0].tag = tag;
				cache[max_chk][0].data = data;
				timer[max_chk][0] = count;
			}
		}
		else
		{
			tag = address >> block_num;
			index = tag&location_bit;
			tag = tag >> tag_bit;

			for (i = 0; i < mode; i++)
			{
				if (cache[index][i].tag == tag)
				{
					if (cache[index][i].data == data) break;
					else
					{
						miss_count++;
						cache[index][i].tag = tag;
						cache[index][i].data = data;
						timer[index][i] = count;
						break;
					}
				}
				if (cache[index][i].tag == 0)
				{
					miss_count++;
					cache[index][i].tag = tag;
					cache[index][i].data = data;
					timer[index][i] = count;
					break;
				}
				if (i == 0) max_chk = i;
				if (lru == 1)
				{
					if (timer[index][max_chk] > timer[index][i]) max_chk = i;
				}
				else
				{
					if (timer[index][max_chk] < timer[index][i]) max_chk = i;
				}
			}
			if (i == mode)
			{
				miss_count++;
				cache[index][max_chk].tag = tag;
				cache[index][max_chk].data = data;
				timer[index][max_chk] = count;
			}
		}
	}
	end = clock();

	printf("- Cache size: %d(KB), block size: %d bytes, input file: %s, LRU: ", cache_size, block_size, argv[4]);
	if (lru == 0) printf("OFF\n");
	else printf("ON\n");
	if (mode == 0)	printf("- directed mapped, ");
	else if (mode == -1) printf("- fully associative, ");
	else printf("- %d-way set associative, ", mode);
	printf("cache miss rate: %lf%%\n", (double)miss_count / (double)count * 100);
	printf("Elapsed time: %lf seconds.\n", (end - start) / CLOCKS_PER_SEC);

	fclose(in);
	return 0;
}
