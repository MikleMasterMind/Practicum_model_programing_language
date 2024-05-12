program

record point
begin
    x, y : int
end;

var 
    a, b : int,
    c, d : point;
begin
    a := 1;
    while (a != 0) do begin
        if (a = 9) then begin
            write(9000);
        end; else begin
            write(800);
        end;
        read(a);
        write(a);
    end;
    c.x := a + 4;
    c.y := c.x * 100;
    d := c;
    write(d.y);
    write(d.x);
end