/*--------------------------------------
  ゲーム制作 ： 基本ソース
  氏名		 ： 2416026 中川原 諒
----------------------------------------*/

#include<DxLib.h> //DXライブラリを使用


/* -----<<< 定数定義 >>>----- */

/* <<< システム系 >>> */

//画面サイズ (初期値：X >>> 640,Y >>> 460)
const int SCREEN_SIZE_X = 640;
const int SCREEN_SIZE_Y = 460;


/* <<< プレイヤー系 >>> */

//移動量 (最小値：0)
const int MOVE_SPEED_X = 7;
const int MOVE_SPEED_Y = 7;
//画面場外制限値 (最小値：0)
const int VIEW_RANGE_X = 7;
const int VIEW_RANGE_Y = 7;
//画像サイズ (最小値：プレイヤー画像ピクセルサイズ(32))
const int PLAYER_IMAGE_SIZE_X = 32;
const int PLAYER_IMAGE_SIZE_Y = 32;
//最大無敵時間
const int INVINCIBLE_MAX_TIME = 300;


/* <<< 弾丸系 >>> */

const int SHOT_MOVE_Y		= 8; //発射速度
const int SHOT_DAMAGE_VALUE = 1; //弾丸攻撃力

//画像サイズ (最小値：ピクセルサイズ(32))
const int SHOT_IMAGE_SIZE_X = 32;
const int SHOT_IMAGE_SIZE_Y = 32;

const int MAX_SHOT_VALUE		= 35;	//最大弾丸数
const int SHOT_RECAST_TIME		= 6;	//連射間隔
const int SHOT_NOT_ACTIVE_POS_X = -100; //未発射時の位置(x)


/* <<< 拡散弾系 >>> */

const int DIFFUSION_SHOT_DAMAGE_VALUE = 1; //拡散弾攻撃力

const int DIFFUSION_SHOT_POS_X		  = 5;   //発射位置をずらす値
const int DIFFUSION_SHOT_MOVE_Y	      = 8;   //発射速度
const int DIFFUSION_SHOT_RECAST_TIME  = 150; //発射頻度
const int DIFFUSION_SHOT_BULLET_VALUE = 5;   //左右別の弾丸数

const int DIFFUSION_SHOT_ANGLE_UP_VALUE = 1; //発射角度増加倍率


/* <<< 敵系 >>> */

const int ENEMY_COUNT = 10; //敵の数

const int ENEMY_MAX_HP = 15; //敵HP

//移動速度
const int ENEMY_MOVE_X = 1;
const int ENEMY_MOVE_Y = 1;

//初期生成地点
const int ENEMY_START_POS_X = 100;
const int ENEMY_START_POS_Y = 50;

//画像サイズ
const int ENEMY_IMAGE_SIZE_X = 64;
const int ENEMY_IMAGE_SIZE_Y = 64;

const int ENEMY_MOVE_LIMIT		   = 0;  //移動制限範囲
const int ENEMY_DAMAGE_IMAGE_ACTIVE_TIME = 3; //敵のダメージ演出時間


/* <<< 爆発系 >>> */

//全体画像サイズ
const int BLAST_MAX_IMAGE_SIZE_X = 576; //縦
const int BLAST_MAX_IMAGE_SIZE_Y = 384; //横

//画像分割数
const int BLAST_IMAGE_COUNT_X = 6; //縦
const int BLAST_IMAGE_COUNT_Y = 4; //横

//各画像のサイズ
const int BLAST_IMAGE_SIZE_X = 96; //縦
const int BLAST_IMAGE_SIZE_Y = 96; //横

const int MAX_BLAST_VALUE	= 24; //爆発画像
const int BLAST_FRAME_VALUE = 2;  //画像フレーム倍率


/* <<< 背景系 >>>*/

//画像サイズ
const int BACKGROUND_IMAGE_SIZE_X = 640;
const int BACKGROUND_IMAGE_SIZE_Y = 960;
//画像移動速度
const int BACKGROUND_MOVE_SPEED = 3;


/* ----- <<< 変数定義 >>> ----- */

int _gameoverCurWaitTime; //現在ゲームオーバー時 待機時間
bool _gameActiveFlag; //ゲーム有効化フラグ(true：Active, false：NotActive)


/* <<< プレイヤー系 >>> */

int _playerCurImage; //画像ハンドル
int _playerNormalImage; //通常画像
int _playerInvImage;	//無敵画像

// プレイヤーの座標 (画面サイズ÷2)×(画像サイズ÷2)
int _playerPosX; //プレイヤー位置(x)
int _playerPosY; //プレイヤー位置(y)

int _playerCurInvTime;		  //現在無敵時間
bool _playerInvActiveFlag;	  //無敵有効化フラグ

bool _playerActiveFlag;  //移動フラグ


/* <<< 弾丸系 >>> */

int _bulletImage;  //弾丸画像ハンドル

//各弾丸の位置
int _bulletsPosX[MAX_SHOT_VALUE]; //弾丸位置(x)
int _bulletsPosY[MAX_SHOT_VALUE]; //弾丸位置(y)

int _bulletsCurRecastTime; //現在発射間隔時間
bool _bulletsShotFlag[MAX_SHOT_VALUE]; //各弾丸の 発射フラグ


/* <<< 拡散弾系 >>> */

int _difBulletImage; //拡散弾ハンドル

//左側の弾丸位置
int _difLeftBulletsPosX[DIFFUSION_SHOT_BULLET_VALUE]; //弾丸位置(x)
int _difLeftBulletsPosY[DIFFUSION_SHOT_BULLET_VALUE]; //弾丸位置(y)
int _difLeftBulletsActiveFlag[DIFFUSION_SHOT_BULLET_VALUE]; //弾丸有効中判定フラグ(true：NowVislble, false：NowInvisible)

//右側の弾丸位置
int _difRightBulletsPosX[DIFFUSION_SHOT_BULLET_VALUE]; //弾丸位置(x)
int _difRightBulletsPosY[DIFFUSION_SHOT_BULLET_VALUE]; //弾丸位置(y)
int _difRightBulletsActiveFlag[DIFFUSION_SHOT_BULLET_VALUE]; //弾丸有効中判定フラグ(true：NowVislble, false：NowInvisible)

int _difShotCurRecastTime; //現在拡散弾 発射可能待ち時間


/* <<< 敵系 >>> */

//画像
int _enemyNormalImage;	   //敵通常画像
int _enemyDamageImage; //敵被ダメージ画像

//移動制限地点
int _enemyMoveLeftLimit;  //移動制限地点(左)
int _enemyMoveRightLimit; //移動制限地点(右)

int _enemyCurImage[ENEMY_COUNT]; //現在の敵画像
int _enemysCurHP[ENEMY_COUNT]; //各敵の現在HP
int _enemysPosX[ENEMY_COUNT];  //各敵の位置(x)
int _enemysPosY[ENEMY_COUNT];  //各敵の位置(y)

int _enemyCurDamageVisibleTime[ENEMY_COUNT]; //敵の現在ダメージ時間

bool _enemyLeftMoveFlags[ENEMY_COUNT]; //敵の左移動フラグ(true：LeftMove, false：RightMove)
bool _enemyActiveFlags[ENEMY_COUNT];   //敵の有効化フラグ(true：Active, false：NotActive)


/* <<< 撃破系 >>> */
//爆発地点
int _blastPosX;
int _blastPosY;

int _blastNum; //描画する番号
int _blastImages[MAX_BLAST_VALUE * BLAST_FRAME_VALUE]; //爆破画像 [画像数 × フレーム倍数]
bool _blastActiveFlag; //爆破演出有効化フラグ(true：Active, false：NotActive)


/* <<< 背景系 >>> */
int _backGroundImage; //背景画像
int _backGroundPosY;  //背景位置


/* <<< 音声系 >>> */
//BGM
int _bgmSound; //BGM音源


/* ------- <<< WinMain関数 >>> --------*/
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	/* <<< システム処理 >>> */

	SetWindowText("2416026 中川原 諒");				//ゲームウィンドウのタイトル
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 32);	//ゲームウィンドウのサイズと色モードの設定
	ChangeWindowMode(true);							//ゲームウィンドウの表示方法(false：フルスクリーン)

	if (DxLib_Init() == -1)
	{
		//TRACE("DxLib の初期化失敗");
		return -1;		//DXライブラリの初期化失敗時 システム終了
	}

	//_dbSetup(640, 480, 255);

	/* ------- <<< グラフィックの登録 >>> ----------*/

	/* <<< 画像読み込み(相対パス) >>> */

	//プレイヤー通常画像
	_playerNormalImage = LoadGraph("image/player.png");
	if (_playerNormalImage == -1)
	{
		OutputDebugString("プレイヤー通常画像の読み込まれていません\n");
		return -1;
	}

	//プレイヤー無敵画像
	_playerInvImage = LoadGraph("image/player_Invincible.png");
	if (_playerInvImage == -1) 
	{
		OutputDebugString("プレイヤー無敵画像が読み込まれていません\n");
		return -1;
	}

	//弾丸画像
	_bulletImage = LoadGraph("image/shot.png");
	if (_bulletImage == -1)
	{
		OutputDebugString("弾丸画像が読み込まれていません\n");
		return -1;
	}

	//拡散弾画像
	_difBulletImage = LoadGraph("image/shot_diffusion.png");
	if (_difBulletImage == -1)
	{
		OutputDebugString("弾丸画像が読み込まれていません\n");
		return -1;
	}

	//敵通常画像
	_enemyNormalImage = LoadGraph("image/enemy1.png");
	if (_enemyNormalImage == -1)
	{
		OutputDebugString("敵画像が読み込まれていません\n");
		return -1;
	}

	//敵被ダメージ画像
	_enemyDamageImage = LoadGraph("image/enemy1_damage.png");
	if (_enemyDamageImage == -1)
	{
		OutputDebugString("敵のダメージ画像 が割り当てられていません");
		return -1;
	}

	//背景画像
	_backGroundImage = LoadGraph("image/haikei.png");
	if (_backGroundImage == -1)
	{
		OutputDebugString("背景画像 が読み込まれていません\n");
		return -1;
	}

	//分割読み込み[画像ハンドル, 分割数, 横の数, 縦の数, 各画像サイズ(x), 画像サイズ(y), 割り当て用配列]
	int blast = LoadDivGraph("image/blast.png", MAX_BLAST_VALUE, 6, 4, BLAST_IMAGE_SIZE_X, BLAST_IMAGE_SIZE_Y, _blastImages);
	if (blast == -1)
	{
		OutputDebugString("爆発画像 が読み込まれていません\n");
		return -1;
	}

	/* <<< 音源読み込み >>> */

	//BGM
	_bgmSound = LoadSoundMem("sound/bgm/BGMSound.mp3");
	if (_bgmSound == -1)
	{
		OutputDebugString("BGM音源 が読み込まれていません\n");
		return -1;
	}


	/*----- <<< 変数の初期化 >>> ------*/

	/* << システム系初期化処理 >> */
	_gameoverCurWaitTime = 0; //待機時間 初期化
	_gameActiveFlag = true;   //ゲーム 有効化


	/* << プレイヤー処理 >> */

	_playerActiveFlag = true; //プレイヤー 有効化
	_playerCurImage = _playerNormalImage; //プレイヤー画像 通常化

	//プレイヤー初期地点 を 画面中央配置 [(画面サイズ ÷ ２) - (画像ピクセルサイズ ÷ ２)]
	_playerPosY = (SCREEN_SIZE_Y / 2) - (PLAYER_IMAGE_SIZE_Y / 2);
	_playerPosX = (SCREEN_SIZE_X / 2) - (PLAYER_IMAGE_SIZE_X / 2);
	//無敵時間 初期化
	_playerCurInvTime = INVINCIBLE_MAX_TIME;
	_playerInvActiveFlag = false;


	/* << 弾丸初期化 >> */

	for (int i = 0; i < MAX_SHOT_VALUE; i++)
	{
		//弾丸位置 初期化
		_bulletsPosX[i] = SHOT_NOT_ACTIVE_POS_X;
		_bulletsPosY[i] = _playerPosY - SHOT_IMAGE_SIZE_Y;

		_bulletsShotFlag[i] = false; //発射フラグ 無効化
	}
	_bulletsCurRecastTime = 0; //連射制限時間 初期化


	/* << 拡散弾初期化 >> */

	//左拡散弾
	for (int left = 0; left < DIFFUSION_SHOT_BULLET_VALUE; ++left)
	{
		//拡散弾位置 初期化
		_difLeftBulletsPosX[left] = SHOT_NOT_ACTIVE_POS_X;
		_difLeftBulletsPosY[left] = _playerPosY - SHOT_IMAGE_SIZE_Y;
		//拡散弾 無効化
		_difLeftBulletsActiveFlag[left] = false;
	}
	//右拡散弾
	for (int right = 0; right < DIFFUSION_SHOT_BULLET_VALUE; ++right)
	{
		//拡散弾位置 初期化
		_difRightBulletsPosX[right] = SHOT_NOT_ACTIVE_POS_X;
		_difRightBulletsPosY[right] = _playerPosY - SHOT_IMAGE_SIZE_Y;
		//拡散弾 無効化
		_difRightBulletsActiveFlag[right] = false;
	}
	_difShotCurRecastTime = 0; //発射制限時間 初期化


	/* << 敵初期化 >> */

	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		_enemyCurImage[i] = _enemyNormalImage;
		_enemysCurHP[i]   = ENEMY_MAX_HP; //HP初期化
		_enemyCurDamageVisibleTime[i] = 0; //現在

		//敵位置 ランダムで初期化
		_enemysPosX[i] = GetRand(SCREEN_SIZE_X - ENEMY_IMAGE_SIZE_X);
		_enemysPosY[i] = -ENEMY_IMAGE_SIZE_Y;

		_enemyLeftMoveFlags[i] = GetRand(1); //敵の左移動をランダムで (有効化 or 無効化)
		_enemyActiveFlags[i]   = true;		 //敵 有効化
	}
	//敵の移動制限地点初期化
	_enemyMoveLeftLimit = ENEMY_MOVE_LIMIT;										//移動制限地点(左)
	_enemyMoveRightLimit = SCREEN_SIZE_X - (ENEMY_IMAGE_SIZE_X + ENEMY_MOVE_LIMIT); //移動制限地点(右)


	/* <<< 爆破系初期化 >>> */

	_blastNum = 0;			  //描画番号 初期化
	_blastPosX = 0;			  //爆破位置 初期化
	_blastActiveFlag = false; //爆破 無効化


	/* <<< 背景系初期化 >>> */

	_backGroundPosY = -(BACKGROUND_IMAGE_SIZE_Y - SCREEN_SIZE_Y);


	/* <<< 音声処理 >>> */

	PlaySoundMem(_bgmSound, DX_PLAYTYPE_LOOP, true); //BGM再生


	/*--------- <<< ゲームループ >>> --------------*/
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		//_dbgStartDrow();

		/* ----- <<< ゲームのメイン処理 >>> -------*/

		/* <<< プレイヤー系 >>> */

		/* << プレイヤー移動処理 >> */
		if (_playerActiveFlag == true)
		{
			//上移動
			if (CheckHitKey(KEY_INPUT_W) == 1 && _playerPosY > VIEW_RANGE_Y ||
				CheckHitKey(KEY_INPUT_UP) == 1 && _playerPosY > VIEW_RANGE_Y)
			{
				_playerPosY -= MOVE_SPEED_Y;
			}

			//下移動
			if (CheckHitKey(KEY_INPUT_S) == 1 &&
				_playerPosY < (SCREEN_SIZE_Y - PLAYER_IMAGE_SIZE_Y) - VIEW_RANGE_Y ||
				CheckHitKey(KEY_INPUT_DOWN) == 1 &&
				_playerPosY < (SCREEN_SIZE_Y - PLAYER_IMAGE_SIZE_Y) - VIEW_RANGE_Y)
			{
				_playerPosY += MOVE_SPEED_Y;
			}

			//左移動
			if (CheckHitKey(KEY_INPUT_A)    == 1 && _playerPosX > VIEW_RANGE_X ||
				CheckHitKey(KEY_INPUT_LEFT) == 1 && _playerPosX > VIEW_RANGE_X)
			{
				_playerPosX -= MOVE_SPEED_X;
			}

			//右移動
			if (CheckHitKey(KEY_INPUT_D) == 1 &&
				_playerPosX < (SCREEN_SIZE_X - PLAYER_IMAGE_SIZE_X) - VIEW_RANGE_X ||
				CheckHitKey(KEY_INPUT_RIGHT) == 1 &&
				_playerPosX < (SCREEN_SIZE_X - PLAYER_IMAGE_SIZE_X) - VIEW_RANGE_X)
			{
				_playerPosX += MOVE_SPEED_X;
			}

			/* << プレイヤー移動制限処理 >> */
			//上
			if (_playerPosY < VIEW_RANGE_Y)
			{
				_playerPosY = VIEW_RANGE_Y;
			}

			//下 [(画像サイズ - プレイヤー画像サイズ) - 描画範囲]
			if (_playerPosY > (SCREEN_SIZE_Y - PLAYER_IMAGE_SIZE_Y) - VIEW_RANGE_Y)
			{
				_playerPosY = (SCREEN_SIZE_Y - PLAYER_IMAGE_SIZE_Y) - VIEW_RANGE_Y;
			}

			//右
			if (_playerPosX < VIEW_RANGE_X)
			{
				_playerPosX = VIEW_RANGE_X;
			}

			//左 [(画像サイズ - プレイヤー画像サイズ) - 描画範囲]
			if (_playerPosX > (SCREEN_SIZE_X - PLAYER_IMAGE_SIZE_X) - VIEW_RANGE_X)
			{
				_playerPosX = (SCREEN_SIZE_X - PLAYER_IMAGE_SIZE_X) - VIEW_RANGE_X;
			}
		}
		else if (CheckHitKey(KEY_INPUT_R) == 1)
		{
			_playerActiveFlag    = true; //プレイヤー 有効化

			_playerCurInvTime	 = 0;	 //無敵時間 初期化
			_playerInvActiveFlag = true; //プレイヤー 一時無敵化
		}

		//無敵化処理
		if (_playerInvActiveFlag == true) 
		{
			//無敵時間 有効化
			if (_playerCurInvTime < INVINCIBLE_MAX_TIME)
			{
				_playerCurInvTime += 1;

				//無敵画像に遷移
				if (_playerCurImage != _playerInvImage) 
				{
					_playerCurImage = _playerInvImage;
				}
			}
			//無敵化 無効化
			else
			{
				_playerInvActiveFlag = false;		  //無敵化 無効化
				_playerCurImage = _playerNormalImage; //通常画像に遷移
			}
		}
		

		/* << 発射処理 >> */

			//射出制限
		if (_bulletsCurRecastTime < SHOT_RECAST_TIME)
		{
			_bulletsCurRecastTime += 1; //射出制限時間増加処理
		}
		//発射処理
		else
		{
			_bulletsCurRecastTime = 0; //連射間隔時間初期化

			for (int i = 0; i < MAX_SHOT_VALUE; i++)
			{
				if (CheckHitKey(KEY_INPUT_SPACE) == 1 && _bulletsShotFlag[i] == false && _playerActiveFlag == true||
					CheckHitKey(KEY_INPUT_Z) == 1 && _bulletsShotFlag[i] == false && _playerActiveFlag == true)
				{
					//プレイヤー位置取得
					_bulletsPosX[i] = _playerPosX;
					_bulletsPosY[i] = _playerPosY - SHOT_IMAGE_SIZE_Y;

					_bulletsShotFlag[i] = true; //弾丸射出有効化フラグ 有効化
					break;
				}
			}
		}
		//連射処理
		for (int i = 0; i < MAX_SHOT_VALUE; i++)
		{
			if (_bulletsShotFlag[i] == true)
			{
				_bulletsPosY[i] -= SHOT_MOVE_Y;

				/* 画面外通過 or 敵描画時 */
				if (_bulletsPosY[i] < -SHOT_IMAGE_SIZE_Y)
				{
					_bulletsShotFlag[i] = false;
				}
			}
		}


		/* ----- <<< 拡散弾発射処理 >>> ----- */

		//射出制限
		if (_difShotCurRecastTime < DIFFUSION_SHOT_RECAST_TIME)
		{
			_difShotCurRecastTime += 1;  //射出制限時間増加処理
		}
		//射出有効化
		else
		{
			_difShotCurRecastTime = 0; //現在拡散弾発射制限時間 初期化

			if (CheckHitKey(KEY_INPUT_SPACE) == 1  && _playerActiveFlag == true||
				CheckHitKey(KEY_INPUT_Z) == 1 && _playerActiveFlag == true)
			{
				//左側射出有効化 処理
				for (int left = 0; left < DIFFUSION_SHOT_BULLET_VALUE; ++left)
				{
					if (_difLeftBulletsActiveFlag[left] == false)
					{
						//プレイヤー位置取得
						_difLeftBulletsPosX[left] = _playerPosX - DIFFUSION_SHOT_POS_X; //発射位置
						_difLeftBulletsPosY[left] = _playerPosY - SHOT_IMAGE_SIZE_Y;

						_difLeftBulletsActiveFlag[left] = true; //弾丸有効判定フラグ 有効化
					}
				}

				//右側射出有効化 処理
				for (int right = 0; right < DIFFUSION_SHOT_BULLET_VALUE; ++right)
				{
					if (_difRightBulletsActiveFlag[right] == false)
					{
						//プレイヤー位置取得
						_difRightBulletsPosX[right] = _playerPosX + DIFFUSION_SHOT_POS_X; //発射位置
						_difRightBulletsPosY[right] = _playerPosY - SHOT_IMAGE_SIZE_Y;

						_difRightBulletsActiveFlag[right] = true; //弾丸有効判定フラグ 有効化
					}
				}
			}
		}
		//左側射出処理
		for (int left = 0; left < DIFFUSION_SHOT_BULLET_VALUE; ++left)
		{
			if (_difLeftBulletsActiveFlag[left] == true)
			{
				//拡散弾移動
				_difLeftBulletsPosX[left] -= DIFFUSION_SHOT_ANGLE_UP_VALUE * (left + 1); //(発射角度値×2)
				_difLeftBulletsPosY[left] -= DIFFUSION_SHOT_MOVE_Y;

				//位置が画面外時 弾丸有効判定フラグ 無効化
				if (_difLeftBulletsPosX[left] < 0 ||
					_difLeftBulletsPosY[left] < 0)
				{
					_difLeftBulletsActiveFlag[left] = false;
				}
			}
		}
		//右側射出処理
		for (int right = 0; right < DIFFUSION_SHOT_BULLET_VALUE; ++right)
		{
			if (_difRightBulletsActiveFlag[right] == true)
			{
				//拡散弾移動
				_difRightBulletsPosX[right] += DIFFUSION_SHOT_ANGLE_UP_VALUE * (right + 1);
				_difRightBulletsPosY[right] -= DIFFUSION_SHOT_MOVE_Y;

				//位置が画面外時 弾丸有効判定フラグ 無効化
				if (_difRightBulletsPosX[right] < 0 ||
					_difRightBulletsPosY[right] < 0)
				{
					_difRightBulletsActiveFlag[right] = false;
				}
			}
		}


		/* ----- <<< 爆破処理 >>> ----- */
		if (_blastActiveFlag == true)
		{
			if (_blastNum < MAX_BLAST_VALUE * BLAST_FRAME_VALUE)
			{
				_blastNum += 1;
			}
			else
			{
				_blastActiveFlag = false;
				_blastNum = 0;
			}
		}


		/* ----- <<< 当たり判定 >>> ----- */

		/* <<< プレイヤー当たり判定 >>> */

		for (int i = 0; i < ENEMY_COUNT; ++i)
		{
			//[プレイヤーX座標 1< 敵右X座標, 
			// プレイヤーY座標 < 敵下Y座標, 
			// プレイヤー右X座標 > 敵X座標,
			// プレイヤー下Y座標 > 敵Y座標,
			// 接触した敵が有効時, プレイヤー有効時, 無敵化無効時]
			if (_playerPosX <= (_enemysPosX[i] + ENEMY_IMAGE_SIZE_X) &&
				_playerPosY <= (_enemysPosY[i] + ENEMY_IMAGE_SIZE_Y) &&
				(_playerPosX + PLAYER_IMAGE_SIZE_X) >= _enemysPosX[i] &&
				(_playerPosY + PLAYER_IMAGE_SIZE_Y) >= _enemysPosY[i] &&
				_enemyActiveFlags[i] == true &&
				_playerActiveFlag	 == true &&
				_playerInvActiveFlag == false)
			{
				_playerActiveFlag = false; //プレイヤー無効化

				//撃破時のプレイヤー位置を 爆破位置に反映
				_blastPosX = _playerPosX + (PLAYER_IMAGE_SIZE_X / 2) - (BLAST_IMAGE_SIZE_X / 2);
				_blastPosY = _playerPosY + (PLAYER_IMAGE_SIZE_Y / 2) - (BLAST_IMAGE_SIZE_Y / 2);

				_blastActiveFlag = true; //爆破演出 有効化
				_blastNum = 0;			 //爆破演出時間 初期化

				//画面中央にプレイヤーを移動
				_playerPosX = (SCREEN_SIZE_X / 2) - (PLAYER_IMAGE_SIZE_X / 2);
				_playerPosY = (SCREEN_SIZE_Y / 2) - (PLAYER_IMAGE_SIZE_Y / 2);
			}
		}

		/* << 弾丸当たり判定・攻撃判定 >> */

		for (int i = 0; i < MAX_SHOT_VALUE; ++i)
		{
			//敵Hit時
			for (int ene = 0; ene < ENEMY_COUNT; ++ene)
			{
				//弾丸位置が敵とほぼ同一 かつ 敵有効時 かつ 弾丸発射有効時
				if (_bulletsPosX[i] <= (_enemysPosX[ene] + ENEMY_IMAGE_SIZE_X) &&
					_bulletsPosY[i] <= (_enemysPosY[ene] + ENEMY_IMAGE_SIZE_Y) &&
					(_bulletsPosX[i] + SHOT_IMAGE_SIZE_X) >= _enemysPosX[ene] &&
					(_bulletsPosY[i] + SHOT_IMAGE_SIZE_Y) >= _enemysPosY[ene] &&
					_enemyActiveFlags[ene] == true &&
					_bulletsShotFlag[i] == true)
				{
					_bulletsShotFlag[i] = false;				 //連射弾 無効化
					_bulletsPosX[i] = SHOT_NOT_ACTIVE_POS_X; //連射弾 位置無効化

					//敵HPがある時
					if (_enemysCurHP[ene] > 0)
					{
						_enemysCurHP[ene] -= SHOT_DAMAGE_VALUE; //対象敵 被ダメージ処理
						_enemyCurDamageVisibleTime[ene] = ENEMY_DAMAGE_IMAGE_ACTIVE_TIME;
					}

					else
					{
						//対象の敵位置を 爆破位置に反映
						_blastPosX = _enemysPosX[ene] + (ENEMY_IMAGE_SIZE_X / 2) - (BLAST_IMAGE_SIZE_X / 2);
						_blastPosY = _enemysPosY[ene] + (ENEMY_IMAGE_SIZE_Y / 2) - (BLAST_IMAGE_SIZE_Y / 2);

						_blastActiveFlag = true; //爆破演出 有効化
						_blastNum = 0; //爆破演出位置 初期化

						_enemyActiveFlags[ene] = false; //対象敵 無効化
					}
				}
			}
		}

		/* << 拡散弾当たり判定・攻撃判定 >> */

		//左側特殊弾
		for (int left = 0; left < DIFFUSION_SHOT_BULLET_VALUE; ++left)
		{
			for (int ene = 0; ene < ENEMY_COUNT; ++ene)
			{
				if (_difLeftBulletsPosX[left] <= (_enemysPosX[ene] + ENEMY_IMAGE_SIZE_X) &&
					_difLeftBulletsPosY[left] <= (_enemysPosY[ene] + ENEMY_IMAGE_SIZE_Y) &&
					(_difLeftBulletsPosX[left] + SHOT_IMAGE_SIZE_X) >= _enemysPosX[ene] &&
					(_difLeftBulletsPosY[left] + SHOT_IMAGE_SIZE_Y) >= _enemysPosY[ene] &&
					_enemyActiveFlags[ene] == true &&
					_difLeftBulletsActiveFlag[left] == true)
				{
					//敵HPがある時
					if (_enemysCurHP[ene] > 0)
					{
						_enemysCurHP[ene] -= DIFFUSION_SHOT_DAMAGE_VALUE; //対象敵 被ダメージ処理
						_enemyCurDamageVisibleTime[ene] = ENEMY_DAMAGE_IMAGE_ACTIVE_TIME;

						_difLeftBulletsActiveFlag[left] = false;				 //連射弾 無効化
						_difLeftBulletsPosX[left] = SHOT_NOT_ACTIVE_POS_X; //連射弾 位置無効化
					}

					else
					{
						//対象の敵位置を 爆破位置に反映
						_blastPosX = _enemysPosX[ene] + (ENEMY_IMAGE_SIZE_X / 2) - (BLAST_IMAGE_SIZE_X / 2);
						_blastPosY = _enemysPosY[ene] + (ENEMY_IMAGE_SIZE_Y / 2) - (BLAST_IMAGE_SIZE_Y / 2);

						_blastActiveFlag = true; //爆破演出 有効化
						_blastNum = 0; //爆破演出位置 初期化

						_enemyActiveFlags[ene] = false; //対象敵 無効化
					}
				}
			}
		}

		//右側特殊弾
		for (int right = 0; right < DIFFUSION_SHOT_BULLET_VALUE; ++right)
		{
			for (int ene = 0; ene < ENEMY_COUNT; ++ene)
			{
				if (_difRightBulletsPosX[right] <= (_enemysPosX[ene] + ENEMY_IMAGE_SIZE_X) &&
					_difRightBulletsPosY[right] <= (_enemysPosY[ene] + ENEMY_IMAGE_SIZE_Y) &&
					(_difRightBulletsPosX[right] + SHOT_IMAGE_SIZE_X) >= _enemysPosX[ene] &&
					(_difRightBulletsPosY[right] + SHOT_IMAGE_SIZE_Y) >= _enemysPosY[ene] &&
					_enemyActiveFlags[ene] == true &&
					_difRightBulletsActiveFlag[right] == true)
				{
					//敵HPがある時
					if (_enemysCurHP[ene] > 0)
					{
						_enemysCurHP[ene] -= DIFFUSION_SHOT_DAMAGE_VALUE; //対象敵 被ダメージ処理
						_enemyCurDamageVisibleTime[ene] = ENEMY_DAMAGE_IMAGE_ACTIVE_TIME;

						_difRightBulletsActiveFlag[right] = false;				 //連射弾 無効化
						_difRightBulletsPosX[right] = SHOT_NOT_ACTIVE_POS_X; //連射弾 位置無効化
					}

					else
					{
						//対象の敵位置を 爆破位置に反映
						_blastPosX = _enemysPosX[ene] + (ENEMY_IMAGE_SIZE_X / 2) - (BLAST_IMAGE_SIZE_X / 2);
						_blastPosY = _enemysPosY[ene] + (ENEMY_IMAGE_SIZE_Y / 2) - (BLAST_IMAGE_SIZE_Y / 2);

						_blastActiveFlag = true; //爆破演出 有効化
						_blastNum = 0; //爆破演出位置 初期化

						_enemyActiveFlags[ene] = false; //対象敵 無効化
					}
				}
			}
		}


		/* ----- <<< 敵処理 >>> ----- */

		for (int i = 0; i < ENEMY_COUNT; ++i)
		{
			//敵有効時の処理
			if (_enemyActiveFlags[i] == true)
			{
				//左移動フラグ有効時 左移動
				if (_enemyLeftMoveFlags[i] == true)
				{
					_enemysPosX[i] -= ENEMY_MOVE_X;
				}
				//左移動フラグ無効時 右移動
				else
				{
					_enemysPosX[i] += ENEMY_MOVE_X;
				}

				//左の移動制限地点到達時に 右移動にシフト
				if (_enemysPosX[i] < _enemyMoveLeftLimit)
				{
					_enemyLeftMoveFlags[i] = false;
				}

				//右の移動制限地点到達時に 左移動にシフト
				else if (_enemysPosX[i] > _enemyMoveRightLimit)
				{
					_enemyLeftMoveFlags[i] = true;
				}


				//下移動処理
				if (_enemysPosY[i] < SCREEN_SIZE_Y + ENEMY_IMAGE_SIZE_Y)
				{
					_enemysPosY[i] += ENEMY_MOVE_Y;
				}

				//上の画面外に 移動
				else
				{
					_enemysPosY[i] = -ENEMY_IMAGE_SIZE_Y;
				}


				//ダメージ演出有効時 演出
				if (_enemyCurDamageVisibleTime[i] > 0)
				{
					_enemyCurDamageVisibleTime[i] -= 1;

					//ダメージ画像に遷移
					if (_enemyCurImage[i] != _enemyDamageImage)
					{
						_enemyCurImage[i] = _enemyDamageImage;
					}

				}
				//ダメージ演出終了時処理
				else
				{
					//通常画像に遷移
					if (_enemyCurImage[i] != _enemyNormalImage)
					{
						_enemyCurImage[i] = _enemyNormalImage;
					}
				}
			}
			//敵再出現処理
			else
			{
				_enemyActiveFlags[i] = true; //敵 有効化

				_enemysCurHP[i] = ENEMY_MAX_HP; //現在HP 初期化

				_enemysPosX[i] = GetRand(SCREEN_SIZE_X - ENEMY_IMAGE_SIZE_X); //ランダムな左右位置で 初期化
				_enemysPosY[i] = -ENEMY_IMAGE_SIZE_Y; //上画面 位置変更

				_enemyLeftMoveFlags[i] = GetRand(1); //左移動フラグ ランダム初期化[leftMove(0) or rightMove(1)]
			}
		}


		/* <<< 背景処理 >>> */

		//背景移動
		if (_backGroundPosY < SCREEN_SIZE_Y)
		{
			_backGroundPosY += BACKGROUND_MOVE_SPEED;
		}
		//背景位置を初期化 [縦の背景画像サイズ - 縦の画面サイズ]
		if (_backGroundPosY >= SCREEN_SIZE_Y)
		{
			_backGroundPosY = -(BACKGROUND_IMAGE_SIZE_Y - SCREEN_SIZE_Y);
		}


		/* <<< 描画処理 >>> */

		SetDrawScreen(DX_SCREEN_BACK); //描画する画面を裏の画面に設定
		ClearDrawScreen();			   //描画する画面の内容を消去

		//_dbgAddDraw();

		/* 対角線を描画 */
		//DrawLine(0, 0, SCREEN_SIZE_X, SCREEN_SIZE_Y, 0xFFFF00);
		//DrawLine(0, SCREEN_SIZE_Y, SCREEN_SIZE_X, SCREEN_SIZE_Y, 0x00FFFF);

		/* 画像描画処理 */

		//背景描画
		DrawGraph(0, _backGroundPosY, _backGroundImage, true);
		DrawGraph(0, (_backGroundPosY - BACKGROUND_IMAGE_SIZE_Y), _backGroundImage, true);


		//各敵を有効時 描画
		for (int i = 0; i < ENEMY_COUNT; i++)
		{
			if (_enemyActiveFlags[i] == true)
			{
				DrawGraph(_enemysPosX[i], _enemysPosY[i], _enemyCurImage[i], true);
			}
		}

		//弾丸発射フラグ有効時 各弾丸画像描画
		for (int i = 0; i < MAX_SHOT_VALUE; ++i)
		{
			if (_bulletsShotFlag[i] == true)
			{
				DrawGraph(_bulletsPosX[i], _bulletsPosY[i], _bulletImage, true);
			}
		}

		//左側拡散弾有効時 左側弾丸描画
		for (int left = 0; left < DIFFUSION_SHOT_BULLET_VALUE; ++left)
		{
			if (_difLeftBulletsActiveFlag[left] == true)
			{
				DrawGraph(_difLeftBulletsPosX[left], _difLeftBulletsPosY[left], _difBulletImage, true);
			}
		}
		//右側拡散弾有効時 右側弾丸描画
		for (int right = 0; right < DIFFUSION_SHOT_BULLET_VALUE; ++right)
		{
			if (_difRightBulletsActiveFlag[right] == true)
			{
				DrawGraph(_difRightBulletsPosX[right], _difRightBulletsPosY[right], _difBulletImage, true);
			}
		}

		//プレイヤー画像を"背景透明で"描画
		if (_playerActiveFlag == true)
		{
			DrawGraph(_playerPosX, _playerPosY, _playerCurImage, true);
		}

		//爆発描画処理
		if (_blastActiveFlag == true)
		{
			DrawGraph(_blastPosX, _blastPosY, _blastImages[_blastNum / BLAST_FRAME_VALUE], true);
		}


		/* -----<<< デバッグ文字 >>>----- */
		//DrawFormatString(0, 0, 0xFFFFFF,  "playerPos = (X：%d , Y：%d)", _playerPosX, _playerPosY);

		/*
		for (int i = 0; i < MAX_SHOT_VALUE; ++i)
		{
			//弾丸位置の値を可視化
			DrawFormatString(0, 64 + (16 * i), 0xFFFFFF, "bulletPos[%d] = (X:%d, Y:%d)", i,_bulletsPosX[i], _bulletsPosY[i]);
		}*/

		
		//裏の画面を表の画面に瞬間コピー(毎度書き直している)
		ScreenFlip();
	}

	/* ------ システム終了処理 -------- */
	DxLib_End(); //DXライブラリ終了処理
	return 0;    //ゲーム終了
}