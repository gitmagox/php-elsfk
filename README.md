# php-elsfk
##一款移植php扩展的俄罗斯方块游戏
##安装
###git 下
```bash
git clone https://github.com/gitmagox/php-elsfk.git
cd php-elsfk 
```
### 编译安装
```bash
phpize
./configure  
make
make install
```
###打开php-ini
extension = magox.so
###运行启动游戏，CLI模式下运行。
```bash
php -r 'magox(1);'