function ret=median_diff(v)

for i=1:(size(v,1)-1)
	diff(i) = abs(v(i)-v(i+1));
end
ret = median(diff);