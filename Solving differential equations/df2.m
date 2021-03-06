function dy = df2(x,y) 
       % 注意哦，x是自变量，y是因变量，由y1,y2,y3组成 
       dy = zeros(3,1);  % 初始化用来储存因变量一阶导数的列向量（不能写成行向量哦）
       dy(1) = 1-3*y(1)-(1*y(1)*y(2))/(1+y(1))-(2*y(3)*y(1))/(2+y(1));
       dy(2) = 1*1*y(1)*y(2)/(1+y(1))-1*y(2)-1*y(2)*y(2)-1*y(2)*y(3);
       dy(3) = 2*2*y(1)*y(3)/(2+y(1))-1*y(3)-2*y(3)*y(3)-2*y(2)*y(3);
%     上面四行可以写成一行：   dy = [ y(2) * y(3);   -y(1) * y(3);  -0.51 * y(1) * y(2)]
end
