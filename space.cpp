#pragma warning (disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <conio.h>
#include <time.h>
#define up VK_UP
#define left VK_LEFT
#define right VK_RIGHT
#define down VK_DOWN
#define space VK_SPACE


/*51x51*/
//GetAsyncKeyState(down)
void gotoxy(int x, int y)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	_COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(hConsole, pos);
}

void dell(int x, int y)
{
	gotoxy(x, y);
	printf(" ");
}

void mostrar(int x, int y,int val)
{
	gotoxy(x, y);
	printf("%c", val);
}

struct bullet
{
	int x, y;
	bullet *sig;
};
struct enemigo
{
	int x, y;
	enemigo *sig, *prev;
};
bullet * crear(int x, int y)
{
	bullet *p = new bullet;
	p->x = x;
	p->y = y;
	p->sig = NULL;
	return p;
}
void trayectoria(bullet *p)
{
	if (p)
	{
		dell(p->x-1, p->y);
		mostrar(p->x, p->y,196);
		p->x++;
		trayectoria(p->sig);
	}
}
void dispara(bullet **p, int x, int y)
{
	if (!*p)
	{
		*p = crear(x, y);
	}
	else
	{
		bullet *t = *p;
		while (t->sig)
			t = t->sig;
		if(t->x>x+3)
			t->sig= crear(x, y);
	}
}
enemigo *fire(enemigo **p,int x,int y)
{
	if (*p)
		return (*p)->x==x&&(*p)->y == y ? *p : fire(&(*p)->sig, x, y);
	return 0;
}
void matar(enemigo **a, bullet **b)
{
	enemigo *ax = NULL;
	if (*b&&*a)
	{
		if (((*b)->x<90)&&(ax=fire(a, (*b)->x, (*b)->y)))
		{
			bullet *t = *b;
			(*b) = (*b)->sig;
			dell(t->x - 1, t->y);
			dell(t->x, t->y);
			dell(t->x+1 - 1, t->y);
			delete t;
			if (ax->prev)ax->prev->sig = ax->sig;
			if (ax->sig)ax->sig->prev = ax->prev;
			if (!ax->sig && !ax->prev)
				*a = NULL;
			delete ax;
			matar(a, b);
		}
		else
			matar(a, &(*b)->sig);
	}
}
void colision(bullet **p)
{
	bullet *t = *p;
	if (t&&t->x > 90)
	{
		(*p) = (*p)->sig;
		dell(t->x-1, t->y);
		delete t;
	}

}
void mover(enemigo *p,int *x)
{
	while (p)
	{
		dell(p->x, p->y);
		if (*x >= 0)
			p->y = p->y++;
		else
			p->y = p->y--;
		if (*x == 0)
			p->x--;
		p = p->sig;
	}
	if (*x < 5)
		*x=*x+1;
	else if (*x == 5)
		*x = -6;
}
void mostrarenemigos(enemigo *p)
{
	if (p)
	{
		mostrar(p->x, p->y, 254);
		mostrarenemigos(p->sig);
	}
}
enemigo *nuevo()
{
	enemigo *p = NULL, *t = NULL;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			t = new enemigo;
			t->x = i+50;
			t->y = j+5;
			t->sig = p;
			t->prev = NULL;
			if (p) p->prev = t;
			p = t;
		}
	}
	return p;
}
void main()
{
	int x, y,mov;
	bullet *disparo=NULL;
	enemigo *e = nuevo();
	x = y = 10;
	mov = 0;
	while (true)
	{
		dell(x, y);		
		if (GetAsyncKeyState(space))
			dispara(&disparo, x, y);
		if (GetAsyncKeyState(down))
			y++;
		if (GetAsyncKeyState(up))
			y--;
		if (GetAsyncKeyState(left))
			x--;
		if (GetAsyncKeyState(right))
			x++;
		colision(&disparo);
		trayectoria(disparo);
		mostrar(x, y,232);		
		mover(e, &mov);
		if(e)matar(&e,&disparo);
		mostrarenemigos(e);
		
		Sleep(100);
	}
		
	/*system("cls");
	printf("\n\n\t\tGAME OVER");
	getchar();*/
}