clear
sec = 3;
[Ax, Ay, Az] = textread('LOG.TXT', '%f %f %f');

s = size(Ax);
dt = sec/s(1);

j = 2;
for i=[dt:dt:sec-dt]	%Shifting abit to make the order correct
	Vx(j-1) = simpsons(Ax(1:j), 0, i, []);
	Vy(j-1) = simpsons(Ay(1:j), 0, i, []);
	Vz(j-1) = simpsons(Az(1:j), 0, i, []);
	j = j+1;
end

j = 2;
for i=[2*dt:dt:sec-2*dt]	%Shifting abit to make the order correct
	Px(j-1) = simpsons(Vx(1:j), 0, i, []);
	Py(j-1) = simpsons(Vy(1:j), 0, i, []);
	Pz(j-1) = simpsons(Vz(1:j), 0, i, []);
	j = j+1
end

plot3(Px,Py,Pz)