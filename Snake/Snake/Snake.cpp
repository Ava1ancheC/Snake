#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<iostream>
#include<list>
#include<string>
#include <queue>
#include<ctime>
#include"jsoncpp/json.h"
using namespace std;
int n, m;
const int maxn = 25;
const int dx[4] = { -1, 0, 1, 0 };
const int dy[4] = { 0, 1, 0, -1 };
bool invalid[maxn][maxn];
bool ibody[maxn][maxn];


class point
{
public:
	int x, y;
	point(int _x = 0, int _y = 0)
	{
		x = _x;
		y = _y;
	}
	void set(int _x, int _y)
	{
		x = _x;
		y = _y;
	}
	point operator+(const point& b)
	{
		return point(x + b.x, y + b.y);
	}

	bool isvaild()
	{
		return x && y && x <= n && y <= m;
	}
}dir[4];

inline bool inborder(point& i)
{
	return (i.x && i.y && i.x <= n && i.y <= m);
}

list<point> snake[2]; // 0表示自己的蛇，1表示对方的蛇




bool ok(point& i)
{
	return(inborder(i) && !invalid[i.x][i.y] && !ibody[i.x][i.y]);
}


bool whetherGrow(int num)  //本回合是否生长
{
	if (num <= 9) return true;
	if ((num - 9) % 3 == 0) return true;
	return false;
}

void deleteEnd(int id)     //删除蛇尾
{
	point p = snake[id].back();
	ibody[p.x][p.y] = 0;
	snake[id].pop_back();
}

point move(int id, int dire, int num)  //编号为id的蛇朝向dire方向移动一步
{
	point p = *(snake[id].begin());
	int x = p.x + dx[dire];
	int y = p.y + dy[dire];
	snake[id].push_front(point(x, y));
	ibody[x][y] = 1;
	if (!whetherGrow(num)) {
		point tmp = snake[id].back();
		deleteEnd(id);
		return tmp;
	}
	return point();
}

void unmove(int id, point a)
{
	point tmp = snake[id].front();
	ibody[tmp.x][tmp.y] = 0;
	snake[id].pop_front();
	if (a.isvaild())
	{
		snake[id].push_back(a);
		ibody[a.x][a.y] = 1;
	}

}

void outputSnakeBody(int id)    //调试语句
{
	cout << "Snake No." << id << endl;
	for (list<point>::iterator iter = snake[id].begin(); iter != snake[id].end(); ++iter)
		cout << iter->x << " " << iter->y << endl;
	cout << endl;
}

bool isInBody(int x, int y)   //判断(x,y)位置是否有蛇
{
	return ibody[x][y];
	/*
   for (int id = 0; id <= 1; id++)
	   for (list<point>::iterator iter = snake[id].begin(); iter != snake[id].end(); ++iter)
		   if (x == iter->x && y == iter->y)
			   return true;
   return false;
   //*/
}

bool validDirection(int id, int k)  //判断当前移动方向的下一格是否合法
{
	point p = *(snake[id].begin());
	int x = p.x + dx[k];
	int y = p.y + dy[k];
	if (x > n || y > m || x < 1 || y < 1) return false;
	if (invalid[x][y]) return false;
	if (isInBody(x, y)) return false;
	return true;
}

int vala[25][25];
int valb[25][25];
bool visited[maxn][maxn];

inline void visit(point i)
{
	visited[i.x][i.y] = true;
}



void bfs(point now, int val[][])
{
	memset(val, 0, sizeof(val));
	memset(visited, 0, sizeof(visited));
	//visit(now);
	queue<point> q;
	q.push(now);
	point tmp;
	point dr;
	while (!q.empty())
	{

		tmp = q.front();
		q.pop();
		visit(tmp);
		for (int i = 0; i < 4; i++) {
			dr = tmp + dir[i];
			if (ok(dr) && !visited[dr.x][dr.y])
			{
				q.push(dr);
				visit(dr);
				val[dr.x][dr.y] = val[tmp.x][tmp.y] + 1;
			}
		}
	}
	return;
	//int ans=0;
	//dbg();
}




int chksum()
{
	int ans = 0;
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= m; j++)
		{
			ans += vala[i][j] > valb[i][j];
			ans -= vala[i][j] < valb[i][j];
		}
	}
	return ans;
}

void dbgg(string& dbg)
{
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= n; j++)
		{
			dbg += to_string(vala[i][j]);
			dbg += " ";
		}
		dbg += '\n';
	}
	dbg += "-------\n";
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= n; j++)
		{
			dbg += to_string(valb[i][j]);
			dbg += " ";
		}
		dbg += '\n';
	}
}

int ope(int num, string& dbg)
{

	int ans = -1;
	point a = snake[0].front();
	point b = snake[1].front();
	point da, db;
	point isgrowa, isgrowb;
	int maxx = -1 << 30;
	int minn;
	for (int i = 0; i < 4; i++)
	{
		da = a + dir[i];
		if (!ok(da))
			continue;
		minn = 1 << 30;

		dbg += to_string(i);
		dbg += ":\n";

		isgrowa = move(0, i, num);
		for (int j = 0; j < 4; j++)
		{
			db = b + dir[j];
			if (!ok(b)) continue;

			dbg += (to_string(j));
			dbg += ":\n";

			isgrowb = move(1, j, num);
			bfs(da, vala);

			bfs(db, valb);

			dbgg(dbg);

			int tmpval = chksum();
			minn = tmpval < minn ? tmpval : minn;
			unmove(1, isgrowb);
		}
		if (minn > maxx)
		{
			maxx = minn;
			ans = i;
		}
		unmove(0, isgrowa);
	}
	return ans;
}

/*
int final() {
	int flag1 = 0;
	int flag2 = 0;
	for (int i = 0; i < 4; i++) {
		if (validDirection(0, i)) {
			flag1 = 1;//我能走
			break;
		}
	}
	for (int i = 0; i < 4; i++) {
		if (validDirection(1, i)) {
			flag2 = 1;//对面能走
			break;
		}
	}
	if (flag1 && flag2)//未结束
		return 0;
	else if (flag1 && !flag2)//我赢
		return 1;
	else//对面赢
		return -1;
}
int win;
int loss;
int counta;
double getval() {
	if (!counta)return -2e16;
	return (double)win / (double)counta;
}
void MaxMin(int depth, int total) {
	int FINAL = final();
	if (depth == 6 || FINAL)//////////////层数
	{
		counta++;
		win += (FINAL == 1);
		//loss += (final() == -1);
		return;
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (validDirection(0, i) && validDirection(1, j)) {
				//蛇头进步
				list<point>tmpsnake[2];
				tmpsnake[0] = snake[0];
				tmpsnake[1] = snake[1];
				move(0, i, total);
				move(1, j, total);
				MaxMin(depth + 1, total + 1);
				snake[0] = tmpsnake[0];
				snake[1] = tmpsnake[1];
				//蛇头还原
			}
		}
	}
}

int ope(int total) {
	if (final() == 1) {
		for (int i = 0; i < 4; i++) {
			if (validDirection(0, i))
				return i;
		}
	}
	int ans = -1;
	double maxrate = -(2e16);
	for (int i = 0; i < 4; i++) {
		if (!validDirection(0, i)) continue;
		double tmp = 2e16;

		for (int j = 0; j < 4; j++) {
			if (validDirection(0, i) && validDirection(1, j)) {
				//蛇头进步
				win = 0; loss = 0; counta = 0;

				list<point>tmpsnake[2];
				tmpsnake[0] = snake[0];
				tmpsnake[1] = snake[1];
				move(0, i, total);
				move(1, j, total);
				MaxMin(1, total + 1);
				snake[0] = tmpsnake[0];
				snake[1] = tmpsnake[1];
				//蛇头还原
				tmp = min(tmp, getval());
				//cout << tmp << endl;
			}
		}
		//double a = getval();
		if (tmp > maxrate) {
			maxrate = tmp;
			ans = i;
		}
	}
	return ans;
}


int Rand(int p)   //随机生成一个0到p的数字
{
	return rand() * rand() * rand() % p;
}

*/

void ini()
{
	for (int i = 0; i < 4; i++)
	{
		dir[i].set(dx[i], dy[i]);
	}
}

int main()
{
	ini();

	memset(invalid, 0, sizeof(invalid));
	string str;
	string temp;
	while (getline(cin, temp))
		str += temp;

	Json::Reader reader;
	Json::Value input;
	reader.parse(str, input);

	n = input["requests"][(Json::Value::UInt) 0]["height"].asInt();  //棋盘高度
	m = input["requests"][(Json::Value::UInt) 0]["width"].asInt();   //棋盘宽度

	int x = input["requests"][(Json::Value::UInt) 0]["x"].asInt();  //读蛇初始化的信息
	if (x == 1)
	{
		snake[0].push_front(point(1, 1));
		snake[1].push_front(point(n, m));
	}
	else
	{
		snake[1].push_front(point(1, 1));
		snake[0].push_front(point(n, m));
	}
	//处理地图中的障碍物
	int obsCount = input["requests"][(Json::Value::UInt) 0]["obstacle"].size();

	for (int i = 0; i < obsCount; i++)
	{
		int ox = input["requests"][(Json::Value::UInt) 0]["obstacle"][(Json::Value::UInt) i]["x"].asInt();
		int oy = input["requests"][(Json::Value::UInt) 0]["obstacle"][(Json::Value::UInt) i]["y"].asInt();
		invalid[ox][oy] = 1;
	}

	//根据历史信息恢复现场
	int total = input["responses"].size();

	int dire;
	for (int i = 0; i < total; i++)
	{
		dire = input["responses"][i]["direction"].asInt();
		move(0, dire, i);

		dire = input["requests"][i + 1]["direction"].asInt();
		move(1, dire, i);
	}

	if (!whetherGrow(total)) // 本回合两条蛇生长
	{
		deleteEnd(0);
		deleteEnd(1);
	}
	//return 0;
	string dbg;
	//随机做出一个决策
	Json::Value ret;
	ret["response"]["direction"] = ope(total, dbg);////////在这填
	ret["debug"] = dbg;
	Json::FastWriter writer;
	cout << writer.write(ret) << endl;

	return 0;
}