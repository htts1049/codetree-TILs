#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

int l, n, q;

int dy[4] = { -1,0,1,0 };
int dx[4] = { 0,1,0,-1 };

bool visited[31];

typedef struct info {
	int val;
	int id;
}info;

typedef struct knight {
	int r, c, h, w, k;
	int sum;
}knight;

typedef struct cmd {
	int i;
	int d;
}cmd;

info map[41][41];
info backup[41][41];
knight knights[31];
cmd cmds[101];
vector<int> movingKnights;
string str[4] = { "위로", "오른쪽으로", "아래로", "왼쪽으로" };


bool isRange(int y, int x) {
	return y >= 1 && y <= l && x >= 1 && x <= l;
}

void visitedInit() {
	for (int i = 1; i <= n; i++) {
		visited[i] = false;
	}
}

bool isMovable(int id, int d) {
	
	int sy, sx, ey, ex;

	switch (d) {
		case 0:
			sy = knights[id].r - 1;
			sx = knights[id].c;
			ey = knights[id].r - 1;
			ex = knights[id].c + knights[id].w - 1;
			break;

		case 1:
			sy = knights[id].r;
			sx = knights[id].c + knights[id].w;
			ey = knights[id].r + knights[id].h - 1;
			ex = knights[id].c + knights[id].w;
			break;

		case 2:
			sy = knights[id].r + knights[id].h;
			sx = knights[id].c;
			ey = knights[id].r + knights[id].h;
			ex = knights[id].c + knights[id].w - 1;
			break;

		case 3:
			sy = knights[id].r;
			sx = knights[id].c - 1;
			ey = knights[id].r + knights[id].h - 1;
			ex = knights[id].c - 1;
			break;
	}

	bool ret = true;

	for (int y = sy; y <= ey; y++) {
		for (int x = sx; x <= ex; x++) {

			
			// 다음 위치가 격자 밖이거나, 벽이면 무조건 false
			if (!isRange(y, x) || map[y][x].val == 2) {
				/*if (!isRange(y, x)) {
					cout << endl << "isRange ERROR!! : " << y << ", " << x;
				}
				else {
					cout << endl << "Wall ERROR!! : " << y << ", " << x;
				}*/
				return false;
			}
			
			// 아직 체크하지 않은 생존한 기사를 만나면 연쇄 탐색
			if (!visited[map[y][x].id] && knights[map[y][x].id].k > 0 && map[y][x].id > 0) {
				
				visited[map[y][x].id] = true;
				movingKnights.push_back(map[y][x].id);
				
				// cout << " " << map[y][x].id;
				ret = ret && isMovable(map[y][x].id, d);
			}
		}
	}

	return ret;
}

void knightMove(int id, int d) {
	
	knights[id].r += dy[d];
	knights[id].c += dx[d];
}

int main() {

	// 입력
	cin >> l >> n >> q;

	// 입력 - map
	for (int i = 1; i <= l; i++) {
		for (int j = 1; j <= l; j++) {
			cin >> map[i][j].val;		// 0은 빈칸, 1은 함정, 2는 벽
			map[i][j].id = 0;
		}
	}

	// 입력 - 기사
	for (int i = 1; i <= n; i++) {
		cin >> knights[i].r >> knights[i].c >> knights[i].h >> knights[i].w >> knights[i].k;
		knights[i].sum = 0;

		for (int y = knights[i].r; y < knights[i].r + knights[i].h; y++) {
			for (int x = knights[i].c; x < knights[i].c + knights[i].w; x++) {
				map[y][x].id = i;
			}
		}
	}

	// 입력 - 명령
	for (int i = 1; i <= q; i++) {
		cin >> cmds[i].i >> cmds[i].d;
	}

	//// 디버깅
	//cout << endl;
	//for (int y = 1; y <= l; y++) {
	//	for (int x = 1; x <= l; x++) {
	//		cout << map[y][x].id << " ";
	//	}
	//	cout << endl;
	//}
	//cout << endl;


	// 1~q턴 까지 진행
	for (int i = 1; i <= q; i++) {
		int id = cmds[i].i;
		int d = cmds[i].d;

		//cout << i << "턴 : " << i << "번 기사 " << str[d] << "이동 " << endl;

		// 1. id번 기사가 사라졌으면 무시
		if (knights[id].k <= 0) continue;

		// 2. d 방향으로 이동가능한지 체크
		visited[id] = true;
		movingKnights.push_back(id);
		
		// cout << "isMovable : " << id;
		// cout << endl;
		bool flag = isMovable(id, d);

		// 3. 이동 가능하면 데미지 입음
		if (flag) {

			// 기사 이동
			for (int i = 0; i < movingKnights.size(); i++) {
				
				// 이동
				knightMove(movingKnights[i], d);
			}

			

			// 4. 이동을 map에 적용
			for (int y = 0; y < l; y++) {
				for (int x = 0; x < l; x++) {

					// 복사할 자리가 이동할 기사가 아니면 복사
					if (!visited[map[y][x].id]) {
						backup[y][x].id = map[y][x].id;
					}
					else {
						backup[y][x].id = 0;
					}
				}
			}

			// 이동한 기사는 새롭게 적용
			for (int i = 0; i < movingKnights.size(); i++) {
				knight mover = knights[movingKnights[i]];

				for (int y = mover.r; y < mover.r + mover.h; y++) {
					for (int x = mover.c; x < mover.c + mover.w; x++) {
						backup[y][x].id = i;
					}
				}
			}

			// 복사본 저장
			for (int y = 0; y < l; y++) {
				for (int x = 0; x < l; x++) {
					map[y][x].id = backup[y][x].id;
				}
			}

			// 5. 데미지 입음
			for (int i = 0; i < movingKnights.size(); i++) {

				if (i == id) continue;

				knight mover = knights[movingKnights[i]];

				int sum = 0;

				for (int y = mover.r; y < mover.r + mover.h; y++) {
					for (int x = mover.c; x < mover.c + mover.w; x++) {

						// 함정 세기
						if (map[y][x].val == 1) {
							sum++;
						}
					}
				}

				knights[movingKnights[i]].sum += sum;
				knights[movingKnights[i]].k -= sum;
			}
		}

		

		// 초기화
		visitedInit();
		movingKnights.clear();

		// 디버깅
		/*cout << endl;
		for (int y = 1; y <= l; y++) {
			for (int x = 1; x <= l; x++) {
				cout << map[y][x].id << " ";
			}
			cout << endl;
		}*/
		/*cout << "HP : ";
		for (int i = 1; i <= n; i++) {
			cout << knights[i].k << " ";
		}
		cout << endl;*/
		//cout << endl;
	}

	// 생존한 기사의 총 피해 합 출력
	int answer = 0;
	for (int i = 1; i <= n; i++) {
		if (knights[i].k > 0) {
			answer += knights[i].sum;
		}
	}

	cout << answer;

	return 0;
}