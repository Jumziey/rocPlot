function ret=max_diff(v)
max_d = 0;

for i=1:(size(v,1)-1)
	diff = abs(v(i)-v(i+1));
	if diff > max_d
		max_d = diff;
	end
end

ret = max_d;