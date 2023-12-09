%%
clear;clc;

%%
% 显示区域参数
radius = 150/2;
x_range = [45, 195];
y_range = [45, 195];
center = [sum(x_range)/2, sum(y_range)/2];
color = 'y';
% 计算五角星坐标
theta = pi/2 + 2*pi/5*(0:9);
x = center(1) + radius*cos(theta);
y = center(2) + radius*sin(theta);
outerPoints = [x(1:2:end); y(1:2:end)];
% 绘制图像
figure(1);
fill(outerPoints(1,:), outerPoints(2,:), color); axis equal;
title('五角星');
% 扩充数据
fft_data = outerPoints(:, [4 3 2 1 5]);
fft_data = [[fft_data(1, 1) fft_data(1, 1) fft_data(1, 1) fft_data(1, 1) fft_data(1, 2:end)],
            [fft_data(2, 1) fft_data(2, 1) fft_data(2, 1) fft_data(2, 1) fft_data(2, 2:end)]];
fft_data = complex(fft_data(1, :), fft_data(2, :));
raw_data = ifft(fft_data);
