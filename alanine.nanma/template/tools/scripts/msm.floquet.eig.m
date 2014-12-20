fm = load('direct.floquet.out');
fm = fm';
[v e] = eig(fm);
ee = sum(e)';
vv1 = v(:,1);
vv1 = vv1 / sum(vv1);
vv =v(:,1:5);
vv(:,1) = vv1;
save ('floquet.eigvalue.out', 'ee');
save ('floquet.eigvector.out', 'vv');

for ii=1:length(vv(:,2))
	 if vv1(ii) ~= 0
	 vv(ii,2) = vv(ii,2)./vv1(ii);
end
end
for ii=1:length(vv(:,3))
	 if vv1(ii) ~= 0
	 vv(ii,3) = vv(ii,3)./vv1(ii);
end
end
for ii=1:length(vv(:,4))
	 if vv1(ii) ~= 0
	 vv(ii,4) = vv(ii,4)./vv1(ii);
end
end
for ii=1:length(vv(:,5))
	 if vv1(ii) ~= 0
	 vv(ii,5) = vv(ii,5)./vv1(ii);
end
end

save ('floquet.eigvector.l.out', 'vv');
