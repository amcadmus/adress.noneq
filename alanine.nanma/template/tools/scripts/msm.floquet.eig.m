fm = load('floque.B.out');
fm = fm';
[v e] = eig(fm);
ee = sum(e)';
vv =v(:,1:5);
save ('floquet.eigvalue.out', 'ee');
save ('floquet.eigvector.out', 'vv');
