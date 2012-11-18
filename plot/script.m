clear
sec = 3;
[x, y, z] = textread('LOG.TXT', '%f %f %f');

bull = size(x)/sec;
dt = 1/bull(1);

Vx = zeros(size(x))

sum = 0;
for i = [2:1:(size(x)-1)]
	
	
end