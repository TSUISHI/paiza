#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// ***********************
// for debug
#define DEBUGs


#define NOP do{}while(0)
#ifdef DEBUG
#define TRACE(...) do{printf(__VA_ARGS__);fflush(stdout);}while(0)
#define TRACECR do{printf("\n");fflush(stdout);}while(0)
#else
#define TRACE(...) NOP
#define TRACECR NOP
#endif

#define PRINCR printf("\n")
#define NOCR(strig) do{char *p;p=strchr(strig,'\n');if(p)*p='\0';}while(0)
// The out-of-date function
#define	asctime(...)	asctime_s(...)
#define	atof(a)		strtod(a,'\0')
#define	atoi(a)		((int)strtol(a,'\0'))
#define	atol(a)		strtol(a,'\0')
#define	ctime(...)		ctime_s(...)
//#define	fopen(...)		fopen_s(...)
//#define	freopen(...)	freopen_s(...)
//#define	rewind(a)		fseek(a,0L,SEEK_SET)
//#define	setbuf(a,b)		setvbuf(a,b,_IOFBF,BUFSIZ)

// for stdio
#define INPUT(str) do{char *p;fgets(str,sizeof(str),stdin);p=strchr(str,'\n');if(p)*p='\0';}while(0)
static char *getinput( char* str );

// for readaility
typedef	long long	lolong;
const int	INF = 1e9;
const int	MOD = 1e9+7;
const lolong	LINF = 1e18;
#define	YES(a)	printf("%s",((a)?"YES":"NO"))
#define	Yes(a)	printf("%s",((a)?"Yes":"No"))
#define OK(a)	printf("%s",((a)?"OK":"NG"))
#define Ok(a)	printf("%s",((a)?"Ok":"Ng"))
#define	POSSIBLE(a)		printf("%s",((a)?"POSSIBLE":"IMPOSSIBLE"))
#define	Possible(a)		printf("%s",((a)?"Possible":"Impossible"))
#define SWAP(type,a,b) do{type _c;_c=a;a=b;b=_c;}while(0)
#define REP(a,b) for(int a=0;a<(int)(b);++a)
#define REP1(a,b) for(int a=1;a<=(b);++a)
#define FLOOP(a,b) for(a=0;a<(b);++a)
#define FLOOP1(a,b) for(a=1;a<=(b);++a)


// ***********************
// stdioから空白区切りの単語を１つだけ読み取ってくる(可変長入力の場合)
static char *getinput( char* str ) {
	char c;
	char *cp;
	cp = &str[0];
	c = fgetc(stdin);
	while( c != EOF ) {
		if( ( c == ' ')||( c == '\n') ) break;
		*cp++ = c;
		c = fgetc(stdin);
	}
	*cp = '\0';
	return &str[0];
}
// ***********************
// FIFO 先入れ先出しのキューを使う
// ***********************
// ***********************
// 個別リテラル
#define INPBUF (50*2+2)
// ***********************
// 外部変数
// FIFO queue {
#define	FIFOMAX	(50*50*4)
typedef struct FIFOQ1 {
	int	x;
	int	y;
	int	steps;
} XYQSTRCT;
XYQSTRCT	fifoxyxy[FIFOMAX];
int		fifofirst = 0;
int		fifoend	= 0;
//	} FIFO queue
char	map[52][52];
int 	foot[52][52];
// ***********************
void	FifoPush( int x, int y ,int steps ) {
	XYQSTRCT buf;

	buf.x = x;
	buf.y = y;
	buf.steps = steps;
	//TRACE("push(%d,%d[%c],%d)",x,y,map[y][x],steps);
	if( (fifoend+1)%FIFOMAX  == fifofirst ) {
		TRACE("buffer over flow\n");
		return;
	}
	memcpy( &fifoxyxy[ fifoend ] , &buf , sizeof(XYQSTRCT) );
	if( ++fifoend >= FIFOMAX ) fifoend -= FIFOMAX;
}
XYQSTRCT *FifoPop( void ) {
	int	retnum;

	if( fifofirst == fifoend ) {
		TRACE("no data\n");
		return NULL;	// 無し
	}
	retnum = fifofirst;
	if( ++fifofirst >= FIFOMAX ) fifofirst -= FIFOMAX;
	return &fifoxyxy[ retnum ];
}
// ***********************
#define WALL	'#'
void bfs( int startx, int starty, int goalx, int goaly ) {
	int	xx,yy;
	XYQSTRCT	*xy;
	int	steps;
	int endofbuff = 0;

	xx = startx;
	yy = starty;
	steps = 0;
	foot[yy][xx] = 0;

	do {
		//TRACE("(%d,%d)[%d]:",xx+1,yy+1,steps);
		// push
		steps++;
		if(( map[yy-1][xx] != WALL )&&( foot[yy-1][xx] > steps )) FifoPush(xx,yy-1,steps);
		if(( map[yy+1][xx] != WALL )&&( foot[yy+1][xx] > steps )) FifoPush(xx,yy+1,steps);
		if(( map[yy][xx-1] != WALL )&&( foot[yy][xx-1] > steps )) FifoPush(xx-1,yy,steps);
		if(( map[yy][xx+1] != WALL )&&( foot[yy][xx+1] > steps )) FifoPush(xx+1,yy,steps);

		do {
			if((xy = FifoPop()) == NULL) {
				TRACE("end\n");
				endofbuff = 1;
				break;	// 無くなったら終了
			}
			xx = xy->x;	yy = xy->y;	steps = xy->steps;
		} while( foot[yy][xx] <= steps );	// 無駄だったら破棄
		if( endofbuff ) break;
		//TRACE("(%d,%d)[%d]:",xx+1,yy+1,steps);
		foot[yy][xx] = steps;
	} while(( xx != goalx )||( yy != goaly ));	// ゴールまで
}
// ***********************
int main(void){
	char str[INPBUF];
	int 	maxx,maxy;
	int		startx,starty;
	int		goalx,goaly;
	int 	query;
	int		okflg;
	// init
	REP(y,52) REP(x,52) foot[y][x] = (255*255);

	// ready
	INPUT(str);	sscanf( str , "%d %d", &maxy, &maxx );
	INPUT(str);	sscanf( str , "%d %d", &starty, &startx );
	INPUT(str);	sscanf( str , "%d %d", &goaly, &goalx );
	REP(i,maxy) {
		INPUT( str );
		TRACE("[%s]\n",str);
		sprintf( &map[i][0], "%s", str);
	}
	// trace
	REP(y,maxy) {
		REP(x,maxx) {
			TRACE("%c",map[y][x]);
		}
		TRACECR;
	}

	// start
	startx--;	starty--;
	goalx--;	goaly--;
	bfs( startx, starty, goalx, goaly );
	// 取得した値を標準出力するために"XXXXXX"をどう書き換える？
	printf("%d\n",foot[goaly][goalx]);
	// trace
	REP(y,maxy) {
		REP(x,maxx) {
			if(foot[y][x] >60000 ) {
				TRACE("## ");
			} else {
				TRACE("%02d ",	foot[y][x] );
			}
		}
		TRACECR;
	}
	
	return 0;
}
/*
50 50
2 2
49 49
##################################################
#................................................#
#................................................#
#................................................#
#................................................#
#................................................#
#................................................#
#................................................#
#................................................#
#................................................#
#................................................#
#................................................#
#................................................#
#................................................#
#................................................#
#................................................#
#................................................#
#................................................#
#................................................#
#................................................#
#................................................#
#................................................#
#................................................#
#................................................#
#................................................#
#................................................#
#................................................#
#................................................#
#................................................#
#................................................#
#................................................#
#................................................#
#................................................#
#................................................#
#................................................#
#................................................#
#................................................#
#................................................#
#................................................#
#................................................#
#................................................#
#................................................#
#................................................#
#................................................#
#................................................#
#................................................#
#................................................#
#................................................#
#................................................#
##################################################
94
---
7 8
2 2
4 5
########
#......#
#.######
#..#...#
#..##..#
##.....#
########
11
---
5 8
2 2
2 4
########
#.#....#
#.###..#
#......#
########
10
*/
