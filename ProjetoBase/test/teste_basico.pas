program exemplo75 (input, output);
var p12:integer;
    procedure p1(var a:integer; b:integer);
    var p1:integer;
    begin
        p1(a,a);
        p1(b,b);
    end;

begin
    p12:=0;
    p1(p12,p12);
end.

