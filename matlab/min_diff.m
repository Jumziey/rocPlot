function ret=min_diff(v)
min_d = inf;

for i=1:(size(v,1)-1)
	diff = abs(v(i)-v(i+1));
	if diff < min_d
		min_d = diff;
	end
end

ret = min_d;