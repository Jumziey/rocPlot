clear
close all;

g = 9.82;
acc_scale = 128/g;
data_list = importdata('LOG.TXT');



h  = median_diff(data_list(:,7));
h = h*1e-3;

%%%%%%%% X-axis
acc_x = data_list(:,1);
acc_x = acc_x/acc_scale;
%acc_x = smooth(acc_x, 'rlowess');

%%%%%%%% Y-axis
acc_y = data_list(:,2);
acc_y = acc_y/acc_scale;
%acc_y = smooth(acc_y, 'rlowess');


%%%%%%%%% Z-axis
acc_z = data_list(:,3);
acc_z = (acc_z-(acc_scale*g))/acc_scale;
%acc_z = smooth(acc_z, 'rlowess');



for i=2:size(acc_x, 1)
	speed_x(i-1,1) = trapz(acc_x(1:1:i))*h;
end

for i=2:size(acc_y, 1)
	speed_y(i-1,1) = trapz(acc_y(1:1:i))*h;
end

for i=2:size(acc_z, 1)
	speed_z(i-1,1) = trapz(acc_z(1:1:i))*h;
end

for i=2:size(speed_x, 1)
	pos_x(i-1,1) = trapz(speed_x(1:1:i))*h;
end
for i=2:size(speed_y, 1)
	pos_y(i-1,1) = trapz(speed_y(1:1:i))*h;
end
for i=2:size(speed_z, 1)
	pos_z(i-1,1) = trapz(speed_z(1:1:i))*h;
end

plot3(pos_x,pos_y,pos_z);

%Har en simpsons funk som kan användas
