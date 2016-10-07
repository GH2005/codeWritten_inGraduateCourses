%
% Predicate: component
%
component(X,X) :- bpart(X).
component(X,Y) :- part(X,Z), component(Z,Y).

