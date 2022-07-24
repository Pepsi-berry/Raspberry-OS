#include "type.h"
#include "stdio.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "fs.h"
#include "proc.h"
#include "tty.h"
#include "console.h"
#include "global.h"
#include <stdlib.h>

#define BOUNDARY '#'
#define WALL '*'
#define PATH ' '
#define HERO 'Y'

int main(int argc,char *argv[])
{
    int i;

    int random;
    char* prRand = argv[argc - 1];
    do
    {
        random = random * 10 + *prRand++;
        /* code */
    } while (*prRand);

    if(strcmp(argv[1], "memory") == 0 && argc == 4)
        gameMemory(argv[2][0]-'0', random);
    else if(strcmp(argv[1], "maze") == 0 && argc == 4)
        gameMaze(argv[2][0]-'0', random);
    else if(strcmp(argv[1], "help") == 0)
        showHelp();
    else if(strcmp(argv[1], "clear") == 0)
        clear();
    else
        printf("Use 'echo help' to get more details you loser.\n");
    
    //printf("%d possibilities in just one moment.\n", random);

	printf("\n");

    return 0;
}

int showHelp()
{
    printf("      =======================================================================\n");
	printf("      #                      Welcome  to  RASPBERRYOS                       #\n");
	printf("      #                          ***  *****  ***                            #\n");
	printf("      #                         *****************                           #\n");
    printf("      #                            ***********                              #\n");
	printf("      #                          **   *   *   **                            #\n");
	printf("      #                        **               **                          #\n");
	printf("      #                       **        *        **                         #\n");
	printf("      #                      *     *         *     *                        #\n");
	printf("      #                      *                     *                        #\n");
	printf("      #                       **      *    *     **                         #\n");
	printf("      #                        **               **                          #\n");
	printf("      #                           **   *  *  **                             #\n");
	printf("      #                             *       *                               #\n");
	printf("      #                               *****                                 #\n");
	printf("      #             [COMMAND]                    [FUNCTION]                 #\n");
	printf("      #           $ echo help         |   show command list of game system  #\n");
	printf("      #           $ echo clear        |   clear the cmd                     #\n");
	printf("      #           $ echo memory num   |   Enter memory game                 #\n");
	printf("      #           $ echo maze num     |   Enter maze game                   #\n");
	printf("      #            The higner the num, the higher the difficulty            #\n");
	printf("      #           You can use [CTRL + F1/F2/F3] to switch consoles          #\n");
	printf("      =======================================================================\n");

    //printf("\n\n");
}

int gameMemory(int degree, int random)
{
    int metrix[7*7];
    char shadow[7*7];
    for(int i=0; i<49; i++)
    {
        metrix[i] = 0;
        shadow[i] = '*';
    }
    initMemoryMetrix(degree + 4, random, metrix);
    initMemoryView(degree + 4, metrix);

    delay(100 + 50 * degree);

    int numGuess = getRand(random) % 9 + 1;
    int count = getcount(degree + 4, metrix, numGuess);

    char rdbuf[4];
	int rd_len = 0;

    updateMemoryView(degree + 4, shadow);

    while(count)
    {
        printf("#%d left#Please enter the position of Number %d(Format: x,y) : ", count, numGuess);
        rd_len = read(0, rdbuf, 3);
        rdbuf[rd_len] = 0;
        if(rdbuf[0] == 'q')
            break;
        int position = (int)(rdbuf[0] - '0') * (degree + 4) + (int)(rdbuf[2] - '0');
        if((metrix[position] == numGuess) && (shadow[position] == '*'))
        {
            count--;
            shadow[position] = (char)(numGuess + '0');
            updateMemoryView(degree + 4, shadow);
        }
        else
            printf("The 3,000th wrong attempt, memory instead of guess you idiot.\n");
    }
    clear();
    if(count == 0)
        printf("Congratulation to the most stupid but lucky guy!\n");
    else
        printf("We all have already known, you loser.\n");

    return 0;
}

int getcount(int degree, int metrix[], int numGuess)
{
    int count = 0;
    for(int i = 0; i < degree; i++)
        for(int j = 0; j < degree; j++)
            if(metrix[i * (degree) + j] == numGuess)
                count++;
    return count;
}

void initMemoryMetrix(int degree, int random, int metrix[])
{
    //double rand = sqrt(3);
    for(int i = 0; i < degree; i++)
    {
        for(int j = 0; j < degree; j++)
            metrix[i * degree + j] = getRand(random++) % 9 + 1;
    }

    return;
}

void initMemoryView(int degree, int metrix[])
{
    clear();
    printf("=====");
    for(int i = 0; i < degree; i++)
        printf("==%d==", i);
    printf("\n");
    for(int i = 0; i < degree; i++)
    {
        printf("\n");
        printf("  %d  ", i);
        for(int j = 0; j < degree; j++)
            printf("  %d  ", metrix[i * degree + j]);
        printf("\n");
    }

    return;
}

void updateMemoryView(int degree, char shadow[])
{
    clear();
    printf("=====");
    for(int i = 0; i < degree; i++)
        printf("==%d==", i);
    printf("\n");
    for(int i = 0; i < degree; i++)
    {
        printf("\n");
        printf("  %d  ", i);
        for(int j = 0; j < degree; j++)
            printf("  %c  ", shadow[i * degree + j]);
        printf("\n");
    }

    printf("By the way, you can 'q' to kick your self out when you want to cry.\n");

	for(int i = 0; i + degree < 15 ;i++)
		printf("\n");
}

static int Rank = 2;

int gameMaze(int degree, int random)
{
    char maze[21*21];

    for(int i = 0; i < 21*21; i++)
        maze[i] = WALL;

    initMazeBoundary(maze, degree * 4 + 5);
    //position of starting point & current position
    int curX = random % 3 + 1;
    int curY = 0;

    maze[curX * (degree * 4 + 5) + curY] = WALL;
    initMaze(degree * 4 + 5, maze, random, curX, curY);
    int endY = initMazeEnd(maze, degree * 4 + 5);
    //printf("%d#%d\n", spX, spY);
    maze[curX * (degree * 4 + 5) + curY] = HERO;
    
    updateMazeView(degree * 4 + 5, maze);

    char rdbuf[4];
	int rd_len = 0;

    while(1)
    {
        if(curX == degree * 4 + 4 && curY == endY)
        {
            printf("You really get patient nerd, congratulation.\n");
            return 0;
        }
        printf("Input the direction you will go(W, A, S, D): ");
        rd_len = read(0, rdbuf, 1);
        rdbuf[rd_len] = 0;
        if(rdbuf[0] == 'q')
        {
            printf("That's why you lose every single time.\n");
            break;
        }
		switch (rdbuf[0]) {
		case 'W':
			if(maze[(curX - 1) * (degree * 4 + 5) + curY] == PATH)
            {
                maze[(curX - 1) * (degree * 4 + 5) + curY] = HERO;
                maze[curX * (degree * 4 + 5) + curY] = PATH;
                curX--;
                updateMazeView(degree * 4 + 5, maze);
            }
			break;
		case 'A':
			if(maze[curX * (degree * 4 + 5) + (curY - 1)] == PATH)
            {
                maze[curX * (degree * 4 + 5) + (curY - 1)] = HERO;
                maze[curX * (degree * 4 + 5) + curY] = PATH;
                curY--;
                updateMazeView(degree * 4 + 5, maze);
            }
			break;
		case 'S':
			if(maze[(curX + 1) * (degree * 4 + 5) + curY] == PATH)
            {
                maze[(curX + 1) * (degree * 4 + 5) + curY] = HERO;
                maze[curX * (degree * 4 + 5) + curY] = PATH;
                curX++;
                updateMazeView(degree * 4 + 5, maze);
            }
			break;
		case 'D':
			if(maze[curX * (degree * 4 + 5) + (curY + 1)] == PATH)
            {
                maze[curX * (degree * 4 + 5) + (curY + 1)] = HERO;
                maze[curX * (degree * 4 + 5) + curY] = PATH;
                curY++;
                updateMazeView(degree * 4 + 5, maze);
            }
			break;
		default:
			break;
        }
    }

    return 0;
}
int initMazeEnd(char maze[], int degree)
{
    for (int i = degree / 2; i < degree; i++)
		if(maze[(degree - 2) * degree + i] == PATH)
            {
                maze[(degree - 1) * degree + i] = ' ';
                return i;
            }
}

void initMazeBoundary(char maze[], int degree)
{
    for (int i = 0; i < degree; i++)
    {
		maze[i * degree + 0] = BOUNDARY;
		maze[0 * degree + i] = BOUNDARY;
		maze[i * degree + (degree - 1)] = BOUNDARY;
		maze[(degree - 1) * degree + i] = BOUNDARY;
	}

    return;
}

int initMaze(int degree, char maze[], int random, int x, int y)     //深度优先生成迷宫
{
    int count = 0;      //当前格上下左右存在的Path数量
    if(maze[(x + 1) * degree + y] == PATH)
        count++;
    if(maze[(x - 1) * degree + y] == PATH)
        count++;
    if(maze[x * degree + (y + 1)] == PATH)
        count++;
    if(maze[x * degree + (y - 1)] == PATH)
        count++;
	if (maze[x * degree + y] == WALL) {
		if (count < 2) 
        if(1)
        {
			maze[x * degree + y] = PATH;

			int direction[4] = { 0,1,2,3 };
			for (int i = 4; i > 0; --i) {
				int r = getRand(random++) % i;
                int temp = 0;
                temp = direction[r];
                direction[r] = direction[i - 1];
                direction[i - 1] = temp;

				switch (direction[i - 1]) {
				case 0:
					initMaze(degree, maze, random, x - 1, y);
					break;
				case 1:
					initMaze(degree, maze, random, x + 1, y);
					break;
				case 2:
					initMaze(degree, maze, random, x, y - 1);
					break;
				case 3:
					initMaze(degree, maze, random, x, y + 1);
					break;
				default:
					break;
				}
			}
		}
	}

    return 0;
}

void updateMazeView(int degree, char maze[])
{
    clear();
    for(int i = 0; i < degree; i++)
    {
        for(int k = 0; k < degree; k++)
        {
            printf("%c%c", maze[i * degree + k], maze[i * degree + k]);
            //printf("#%d", degree);
        }
        printf("\n");
    }
    printf("By the way, you can 'q' to kick your self out when you want to cry.\n");

    return;
}

void delay(int time)
{
    int i,j,k;
    for (k=0; k<time; k++)
    {
        for (i=0; i<10; i++)
        {
            for (j=0; j<30000; j++)
            {
                ;
            }
        }
    }
}

void clear()
{
	for(int i = 0; i < 25 ;i++)
		printf("\n");
}

const int randNum[2000] = 
{
	22235,30044,12281,27829,9208,23610,26115,2798,1930,30995,12816,21312,27756,18209,12033,3924,26293,30454,17588,23269,
	26949,7614,10804,27938,31134,27077,22572,4435,22006,57,32674,4800,16778,26672,22753,8142,27444,9964,24247,26732,
	8768,29022,18913,31661,19836,5866,24772,10422,22670,2410,28510,26331,2954,9845,31611,2309,196,9697,2348,16471,
	11491,20870,885,12817,11575,27205,7494,9396,20557,24138,16044,2151,3264,13453,21065,24217,23121,20895,1794,17640,
	8506,31139,23031,10049,13248,13210,31928,23881,1544,29157,27574,14076,6187,9220,18679,15508,18060,5540,20238,9645,
	29802,16089,28747,19366,17585,27048,30255,6986,21782,20680,13602,18864,12130,7081,10344,20575,25870,23478,2642,29878,
	2321,28376,15537,24204,31734,27451,5520,531,15921,30804,25635,24834,15539,14627,16890,26355,19388,11189,1146,25865,
	17393,8686,25230,28887,30126,14213,8858,10877,26269,26960,25777,11796,3464,12738,23815,13389,107,20146,4777,9812,
	18638,5213,30315,10491,21073,29103,26146,25364,10121,2494,28692,798,28938,1688,27309,18469,27333,2366,13428,4579,
	17301,23280,12349,15403,6932,17323,20895,18824,4531,10737,19880,8481,7946,24520,17360,25567,8812,16645,29484,7378,
	25023,1738,20452,20689,27274,29774,3303,12641,2384,16604,28125,30425,19229,25375,30046,9667,15710,8604,17249,17284,
	24565,3200,14327,15936,32141,17564,11833,3064,24107,13230,25470,17653,2054,6626,24274,15722,665,5272,27812,25173,
	20470,616,703,23974,4189,26723,8316,17827,1037,15516,32565,23435,24410,14824,9980,7545,9472,8417,15245,12172,
	31415,2609,5506,24645,11272,11789,31686,19504,20318,17663,4428,25799,16736,26089,19201,24211,27262,1351,511,19092,
	714,31156,30211,24291,18442,10672,21537,12950,23051,18080,5558,28900,12600,14614,19359,18314,25450,8787,10664,5228,
	11010,5898,26183,5760,25654,30567,17035,9704,29864,2128,20047,14174,20797,12779,15539,11989,5676,1888,23757,17757,
	25941,31712,2715,1608,32469,26854,11324,10123,18391,18876,3936,16743,13228,20546,15102,10568,17183,14059,2373,29677,
	3586,29704,7636,25289,2829,12873,3117,16553,2537,12928,16999,29080,4999,31401,3108,14847,9398,30248,16835,273,
	30453,27473,30679,4710,19333,27181,17111,8934,27843,6267,25367,7401,22965,32502,27894,30621,20451,15318,16729,7591,
	10088,16025,27225,9997,13640,28931,12345,27219,10244,29028,31387,10468,4716,16921,20979,23220,6325,4693,5435,23844,
	29643,1058,14313,2342,16104,5026,7198,29857,17254,923,23840,25367,14034,29490,7690,10629,8001,247,12482,2442,
	20967,7970,10540,18380,16357,32028,12445,408,1379,10818,11667,1480,28383,21989,5027,13546,26820,20629,28594,11422,
	3391,12959,1128,16960,17486,19412,2786,4295,10778,16524,27559,7661,12166,30699,990,23185,10504,16227,19134,601,
	16312,18298,27461,7835,30886,24014,9773,12414,18033,2134,4124,18012,22805,26090,27,107,8197,30726,16969,18589,
	12922,14795,25296,16836,4618,10310,9497,12794,31612,8181,31548,27712,14811,13191,4539,32101,12233,24548,17263,8234,
	2838,16749,2498,15523,22161,11909,15647,30432,24094,23376,14759,4301,6409,27322,15398,9047,3738,4284,2885,29392,
	26858,10715,16641,25237,10740,27633,18523,19438,14063,5167,25425,15413,11740,13923,19726,24123,14683,4471,19550,4839,
	22210,3386,19044,32032,17008,17764,16783,1261,32351,17876,6346,5595,9977,23579,16115,7328,2605,8766,6799,2045,
	13505,9912,8636,20713,27114,21951,11878,19947,14618,1992,25278,21960,9753,22861,12776,30046,5975,12238,24820,15568,
	32385,4118,5136,26595,28498,10687,15356,26024,6316,15352,30654,860,21479,14214,16022,16012,15977,16905,26164,636,
	4470,32385,22825,21706,11338,16144,4508,13647,1125,1026,11326,13638,28711,27658,10112,2480,32668,31182,8963,7415,
	22425,21158,13135,25553,17160,23201,7625,31079,28288,20938,4319,18103,537,22185,11398,14646,3454,29196,28390,19642,
	31299,4103,22867,27800,30841,2617,24960,28681,6528,10065,751,13779,3357,13265,1196,8997,13516,6162,18235,2498,
	344,3976,11983,14689,20136,25928,32189,16468,12764,22983,13103,8041,28057,5605,16925,29966,160,20266,13433,25528,
	678,335,3754,14213,17248,24832,1852,3872,24541,27616,26326,3045,16920,14798,9324,19781,16078,29543,4533,15212,
	17805,4555,11232,25890,22286,19364,25731,29935,13764,21349,30420,387,1205,19152,7451,15700,18569,4027,14951,18825,
	32480,17939,19584,12311,16361,17185,14878,18730,7729,15884,20824,4038,14520,27040,2655,10670,13130,23373,25196,9861,
	2316,25459,25335,14899,27990,15069,10538,30346,11968,10635,405,18474,20608,2740,27683,15009,19955,21003,7245,18391,
	15481,30274,18829,13994,16262,25804,25622,19669,758,25320,450,22550,12193,32731,5964,21136,3632,31868,20927,9838,
	19503,19446,9262,21705,5987,4554,23974,971,861,16659,23393,14357,27983,16654,11068,15711,21023,15065,12907,606,
	19130,29819,11372,27627,8875,23845,5689,25441,15596,11960,22978,9287,15099,18566,19866,3375,6705,10077,23793,30685,
	15184,7874,27586,23724,2139,14202,19277,17710,11434,22351,21192,21011,4925,5648,28754,22604,8017,25562,17960,22896,
	21297,25750,10267,23390,22863,9411,3394,25380,9107,2934,18924,5919,5363,11678,9688,19556,26948,26253,8873,863,
	20303,22964,1684,6074,26920,16814,32604,10112,9915,12417,29056,30083,11944,9504,18355,19195,18769,6708,22156,7279,
	560,29211,10367,30169,29551,25649,17932,29189,28594,20303,20022,16698,32312,5522,29495,796,23842,25950,12146,30673,
	32274,5026,18089,19555,12536,27485,26480,1637,28969,5729,225,21210,2226,855,8338,15442,32235,27907,27844,3158,
	32591,26313,3805,23558,16778,4720,8729,6145,980,25383,31356,31621,11902,4549,22539,18136,16330,9813,23099,13542,
	31488,15058,18346,29739,3518,23343,32269,9939,8080,9882,7785,11808,32704,5634,18823,17677,25968,3031,162,11825,
	30181,4369,32354,30949,1318,25459,7182,8617,17606,14609,5885,4548,21758,23824,19021,16939,27647,2477,12504,1444,
	1925,9088,17252,15915,23728,14850,1818,405,31248,29006,24634,29043,20478,14628,32012,19001,4915,30003,27930,28064,
	11667,31384,14492,27720,12618,1698,3124,21154,538,4824,19675,15629,10483,10813,10267,30527,1477,9170,29824,10983,
	21394,26205,30281,24976,12999,6505,23650,24009,11724,26510,2455,3971,27610,13810,32258,10170,1549,29968,29893,3928,
	12986,28344,2559,17750,15106,13655,31992,26660,28327,6265,25247,28235,2591,17036,23607,21073,17127,25575,9805,13788,
	28186,3851,5101,16411,30018,10792,18965,15202,3438,6627,20330,13844,25733,2390,17450,10950,14553,29528,22114,29601,
	3839,20735,18017,3336,31199,4452,3948,27515,10503,16038,2227,26812,12613,29746,4740,3120,17268,2625,9151,12935,
	17814,6818,19338,22688,547,17081,31557,3494,24993,18120,11457,20608,25205,31341,14508,23656,17145,4716,29008,10457,
	4558,17327,21886,8367,6260,21387,28612,21642,21527,32069,13798,11379,28844,20693,31884,23746,31293,28393,21305,8766,
	14465,17965,23676,2439,26800,13770,30314,17841,18837,14636,18251,23090,11749,265,660,7166,2255,7583,31254,19952,
	10452,7741,20340,23660,5337,18948,23002,31811,16251,21999,5100,12502,1186,15377,9725,32127,9561,6174,24706,17964,
	13789,11565,15046,9037,12787,30908,10719,11505,11135,30453,25568,299,18259,31792,25424,2491,31813,18360,16091,17642,
	18564,13266,10568,5558,20780,9979,11691,24367,8215,10866,31708,28449,29687,18934,22823,31731,22177,8401,2373,14799,
	28076,14357,17291,19204,14651,6694,2146,4008,13591,16248,27383,6273,4097,14490,9012,16678,27974,14708,31726,13665,
	25747,27085,29024,26072,13323,7952,5080,12268,22679,25423,688,15586,125,28040,4992,23616,3125,28455,31122,23930,
	23245,406,11470,5273,2128,13719,14189,27672,23043,32046,23259,629,18231,3574,25496,27202,28673,16447,24320,
	30844,7742,7175,17520,22811,22485,12838,14762,20968,26172,5576,30331,10127,30526,2592,2971,22773,15099,14427,4308,
	9419,9008,467,32358,3428,10168,17030,13739,20163,13395,6086,31991,31481,9127,32562,29378,12462,27524,25060,15369,
	8189,24172,24556,29169,20869,9811,28669,4782,25532,23715,10087,31139,12935,25475,15941,8906,7432,25689,9208,15703,
	23991,6029,14292,20888,10421,14737,31256,32288,1117,20766,18724,4132,4123,10944,32486,31525,11649,11668,662,29647,
	16279,27984,8867,12834,29811,7557,19181,7887,19722,18977,27444,28645,22342,31032,8656,30892,20057,6660,32419,12497,
	5456,25898,29261,14020,16363,4670,15774,20831,7596,1674,30033,13512,31534,24843,28151,3736,3512,1981,9265,7485,
	15634,6681,29986,22173,16279,27867,15140,25083,15500,29448,8890,26911,26800,14384,19399,1201,20756,20464,2813,18407,
	18477,24252,24471,12128,10536,8155,29003,30543,25981,3251,10874,7387,4295,2375,17067,3782,24275,14846,19767,20813,
	18423,26278,8261,6480,18341,3768,10233,8883,26943,4830,31209,17924,5933,6066,15569,4446,24622,1481,14471,26765,
	9963,19184,8753,28786,28626,16565,30901,18857,16673,23594,1393,24170,18753,2789,21805,18349,16214,24907,15693,3474,
	12820,32271,16715,18175,14548,18165,27749,23691,15079,25308,29587,29339,16682,5371,13244,16074,1498,23915,24617,19599,
	13994,741,30010,29978,7443,27086,17240,3710,8428,23664,9770,5653,22842,10379,15372,23072,8009,13836,12733,26287,
	22524,31516,30295,14785,26465,29020,27525,683,648,28783,26096,28862,21398,4538,4602,21796,9237,359,4271,7179,
	398,24752,24824,24838,4186,7514,26898,8879,181,25215,13375,22930,23060,1681,14524,30278,4281,6812,14834,2781,
	6532,32459,6896,9087,28684,8238,13593,4689,17134,17338,1818,12955,14949,658,20512,26411,3355,23429,27463,22516,
	22335,32215,32597,13051,26584,12612,22184,29812,30809,32578,29549,31271,629,391,13331,26137,10292,31179,7167,5225,
	25159,5839,5250,10360,26777,2718,14856,30028,2342,22034,2450,17719,24848,28059,614,17587,11247,6972,4100,5265,
	7529,16854,20573,18259,30980,26025,2624,19926,23607,7704,26645,5940,28359,21676,19149,26600,27985,30044,18713,11315,
	16930,29708,11994,18879,8271,19283,4313,13443,21763,22657,18617,19174,17171,14561,27946,15655,15576,3131,6751,1020,
	12349,16541,7873,10873,22807,19291,24549,28468,28085,29525,3003,167,31160,4216,27145,22741,23333,25736,25112,31793,
	5561,28956,2222,8123,28797,28394,19003,31340,4506,19012,1367,17876,3181,18659,10604,12802,5598,15588,11696,20644,
	21230,26896,29783,15215,30096,31328,28166,31169,7098,15040,4490,27013,9403,24852,31690,12120,28879,8825,19450,29161,
	16113,26829,4114,25766,7408,21105,21887,26005,7621,24397,12824,12927,18980,28437,22867,31060,32593,29899,4537,18379,
	12341,902,7831,7753,15537,6257,29175,10756,21298,17942,15054,27240,4230,9437,9079,17850,20974,11471,21600,20731,
	28020,23712,3844,20239,17534,20973,29977,11038,25764,14018,20490,29243,10943,10335,27815,4163,7234,31548,24348,21512,
	27342,74,17183,32431,22016,12156,19566,25175,24462,23430,15225,21500,1426,21221,2651,29850,30501,4639,26127,5525,
	30512,27468,18401,15448,24959,18133,17186,2364,1559,24956,23328,1452,20170,11164,30038,7560,25352,11698,15327,18374,
	15864,5661,1073,8799,27335,21538,3591,1385,23361,25651,11935,20864,3230,12657,15398,32107,28318,12399,19825,9981,
	16600,5945,8685,11064,23118,22165,17881,2489,3910,14472,18098,5968,10581,18460,3055,17390,9661,26209,10223,30660,
	11769,12466,13616,19369,32405,7225,5057,8380,12355,27318,6906,23371,5932,23675,4970,17652,11396,17339,3345,23050,
	13921,23813,26063,1595,8653,4364,5149,5231,17177,937,2555,22002,28789,5155,17449,17622,10942,12973,22938,9619,
	18583,12080,7974,27633,22459,25931,13183,26408,10386,27033,1894,31045,14338,7218,13855,24015,27527,17524,16930,17323,
	21382,6659,2579,15933,26590,24334,2548,20944,29377,28628,15514,7992,7618,27495,12637,29365,21806,22960,10772,14875,
	30112,29364,21825,9776,30672,27413,30856,31574,3756,27100,8038,21866,27562,2933,8735,25323,7516,5627,6392,23391,
	15429,17079,4007,13554,23019,16611,3336,23905,26157,21939,17169,31254,32471,15766,31496,29589,23924,12985,4595,20502,
	22371,15564,6481,8563,9261,11786,28679,2629,24086,29043,26525,7754,28914,3676,18858,8715,27450,1322,20877,28091,0 
};

int getRand(int startNum)
{
	return randNum[startNum%2000];
}

int abs(int num)
{
    return (num >= 0 ? num : num * -1);
}
