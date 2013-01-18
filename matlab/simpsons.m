function integral=simpson(y, h)
% integrerar numeriskt funktionen func.
% på intervallen [a,b] med n antal noder
% med simpsons metoden.

if (mod(y,2)==0)
	error('Måste ha ett ojämt antal noder n');
end

n = size(y,1);
sumy = 0;
for i=1:2:(n-2)
sumy = sumy+y(i)+4*y(i+1)+y(i+2); % by simpsons
end
integral = (h/3)*sumy;
