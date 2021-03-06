[x, y] = ode45('df2', [0 4*pi], [0 1 1]);  % 这里的y是一个有3列的矩阵哦！
plot(x, y(:,1), 'o', x, y(:,2), '*', x, y(:,3), '+') 
legend('y1','y2','y3')  % 加上标注
axis([0, 4*pi, -inf, +inf])  % 设置横坐标范围为0-4pi，纵坐标范围不需要设置，写成-inf到+inf