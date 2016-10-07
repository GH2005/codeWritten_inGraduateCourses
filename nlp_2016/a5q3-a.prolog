child(john, mary).
child(ann, john).
child(tom, john).

grandchild(X, Y) :- child(X, Z), child(Z, Y).
