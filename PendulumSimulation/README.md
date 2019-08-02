## PendulumSimulation
質点と直線によって構成された三重振り子 

### ビルド
* x86でビルドしてください。
* Releaseビルドしないと処理がとても遅くなるので気を付けてください。

### 設定ファイル
初期値や色などを自由に変更できます。  
* デフォルトでは実行ファイルと同じフォルダ内の "pendl_setting.ini" というファイルが読み込まれます。
* コマンドラインから実行する場合、第一引数に設定ファイルのパスを指定できます。
* "pendl_setting_sample.ini" の書き方を参考にしてください。

[size]  
lineWidth = 線分の太さ  
locusWidth = 軌跡の太さ  
circleSize = 質点の大きさ  
circlePartition = 質点を何角形で表現するか  
  
[theta]  
初期角度。根本から順に３つ。  
  
[dtheta]  
初期角速度。根本から順に３つ。  
  
[mass]  
質点の質量。根本から順に３つ。  
  
[length]  
線分の長さ。根本から順に３つ。  
  
[speed]  
repeat = 表示間隔。この回数シミュレーションが行われるたびに表示が更新される。  
sleepms = 時間間隔dt。ミリ秒単位。  
  
[ballColor0]  
振り子の根本の色。RGBの順。  
  
[ballColor1]  
[ballColor2]  
[ballColor3]  
質点の色。RGBの順。根本から順に３つ。  
  
  
[locusColor0]  
[locusColor1]  
[locusColor2]  
質点の軌跡の色。RGBの順。根本から順に３つ。  
  
[lineColor]  
線分の色。RGBの順。  
  
[bgColor]  
背景色。RGBAの順。詳細にはglClearColorの引数。  


### キーバインド 
実行中に指定されたキーを押すと以下のアクションが実行されます。  
スタート時及びリスタート時に与えれた初期値は、コマンドラインに出力されます。
* q, z  
終了  
* s  
一時停止
* p  
軌跡の表示 ON/OFF  
* w, e, r, t, y, f  
リスタート
    * w  
デフォルト設定
    * e  
そのまま
    * r  
棒の長さ・質点の質量/角度・角加速度 をランダムに変更
    * t  
角度・角加速度 をランダムに変更
    * y  
棒の長さ・質点の質量 をランダムに変更
    * f  
設定ファイル再読み込み  

### 使用ライブラリ
* Eigen  
行列計算用
* freeglut  
OpenGL用  
* SimpleIni  
設定ファイル読み込み用

### おまけ
流れ  
1. ラグランジュアンから運動方程式を導出  
2. 変数変換によって一階の連立常微分方程式にする  
3. LU分解で各変数を独立させる  
4. 四次のルンゲ・クッタ法で微分方程式を解く  


### ソースコード
[0V/G2Physics | GitHub](https://github.com/0V/G2Physics)
