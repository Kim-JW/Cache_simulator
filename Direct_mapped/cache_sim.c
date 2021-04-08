#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>

char firstLine[200], cmdLine[200];
int mode, cache_size, block_size, lru, address, data;
int miss_count, count, block_num;
double start, end;

struct
{
	unsigned int index;
	unsigned int tag;
	unsigned int data;
	int valid;
} cache[100000];


int main(int argc, char **argv)
{
	int location_bit, index, tag, temp, tag_bit, i, chk;

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
	tag_bit = 0;
	location_bit = 1;
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
		tag = address >> block_num;
		index = tag&location_bit;
		tag = tag >> tag_bit;
		if (cache[index].tag != tag)
		{
			miss_count++;
			cache[index].valid = 1;
			cache[index].tag = tag;
			cache[index].data = data;
		}
		else if (cache[index].tag == tag && cache[index].data != data)
		{
			miss_count++;
			cache[index].valid = 1;
			cache[index].data = data;
		}
	}
	end = clock();

	printf("- Cache size: %d(KB), block size: %d bytes, input file: %s, LRU: ", cache_size, block_size, argv[4]);
	if (lru == 0) printf("OFF\n");
	else printf("ON\n");
	if (mode == 0)	printf("- directed mapped, ");
	printf("cache miss rate: %lf%%\n", (double)miss_count / (double)count * 100);
	printf("Elapsed time: %lf seconds.\n", (end - start) / CLOCKS_PER_SEC);

	fclose(in);
	return 0;
}
