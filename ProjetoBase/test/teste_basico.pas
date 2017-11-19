program exemplo75 (input, output);
    var a,b:boolean;
    function f1():boolean;
        procedure p2(x:integer);
        begin
            f1:=true;
            x:=3;
        end;
    begin
        f1:=false;
        p2(2);
    end;
    procedure p1(var x:boolean);
    begin
    end;
begin
    a := f1;
    p1(a);
end.

