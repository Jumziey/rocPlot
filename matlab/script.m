clear
g = 9.82;
acc_scale = 128/g;
gyro_scale = 14.375;
data_list = importdata('test_log.txt');



%h  = median_diff(data_list(:,7));
h = 14*10e-3;

gyro_speed_z = data_list(:,6);
gyro_speed_z = gyro_speed_x/gyro_scale;

subplot(1,2,1);
x1 = 1:1:size(gyro_speed_x, 1)

for i=3:(size(gyro_acc_x, 1)-2)
	gyro_speed_x(i-2,1) = simpson(gyro_acc_x(1:1:i), h);
end

bxplot(x1,gyro_acc_x)
%
%for i=3:(size(gyro_speed_x, 1)-2)
%	gyro_x(i-2,1) = simpson(gyro_speed_x(1:1:i), h);
%end
