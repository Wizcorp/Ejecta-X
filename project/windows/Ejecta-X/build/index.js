var ej = new Ejecta.EjectaCore();
var setTimeout = function(cb, t){ return ej.setTimeout(cb, t); };
var canvas = new Ejecta.Canvas();
var context = canvas.getContext('2d');
var img = new Ejecta.Image();

ej.pos_x = 0;
ej.pos_y = 0;

img.onload = function(){
	img.onload = null;
	beginPaint();
};


img.src = 'bg.png';

a = img.width;

function beginPaint(){
	if(ej.pos_x>200)ej.pos_x=0;
	if(ej.pos_y>200)ej.pos_y=0;
	ej.pos_x++;
	ej.pos_y++;
	canvas.drawImage(img, ej.pos_x, ej.pos_y);
	setTimeout(beginPaint, 100);
}
