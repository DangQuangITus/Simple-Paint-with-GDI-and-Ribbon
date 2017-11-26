#pragma once
#include <ObjIdl.h>
#include <gdiplus.h>
#include <math.h>
#include <string>
#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

#define LINE		1
#define RECTANGLE	2
#define ELLIPSE		3
#define SQUARE		4
#define CIRCLE		5
#define SELECTED	6 


namespace Paint
{
	class CStyle
	{
	public:
		DashStyle dStyle;
		Color color;
		int WidthLine;
		//------------------------------------
	public:
		CStyle() {
			dStyle = DashStyleCustom;
			color = color.Black;
			WidthLine = 2;
		}

		void SetDashStyle(DashStyle dstyle) {
			dStyle = dstyle;
		}
		void SetColor(Color cl) {
			color = cl;
		}
		void SetWidthLine(int widthline) {
			WidthLine = widthline;
		}
		~CStyle() {}
	};

	class CShape
	{
	public:
		virtual void Draw(HDC, CStyle) = 0;
		virtual CShape* Create() = 0;
		virtual void SetData(int, int, int, int) = 0;
		virtual void GetData(int&, int&, int&, int&, int&) = 0;
		virtual LPWSTR GetNameClass() = 0;
	};

	class CLine : public CShape
	{
	public:
		int fX;
		int fY;
		int lX;
		int lY;
	public:
		CLine() {}
		void Draw(HDC hdc, CStyle style) {
			Graphics* g = new Graphics(hdc);
			g->SetSmoothingMode(SmoothingModeAntiAlias);
			Pen* pen = new Pen(style.color, style.WidthLine);
			pen->SetDashStyle(style.dStyle);
			g->DrawLine(pen, fX, fY, lX, lY);
		}
		CShape* Create() { return new CLine; }
		void SetData(int a, int b, int c, int d) {
			fX = a;
			fY = b;
			lX = c;
			lY = d;
		}
		void GetData(int& type, int& fx, int& fy, int& lx, int& ly) {
			type = LINE;
			fx = fX;
			fy = fY;
			lx = lX;
			ly = lY;
		}
		~CLine() {}
		LPWSTR GetNameClass() {
			return L"Cline";
		}
	};

	class CRectangle : public CShape
	{
	public:
		int fX;
		int fY;
		int lX;
		int lY;
	public:
		CRectangle() {}
		void Draw(HDC hdc, CStyle style) {
			Graphics* g = new Graphics(hdc);
			g->SetSmoothingMode(SmoothingModeAntiAlias);
			Pen* pen = new Pen(style.color, style.WidthLine);
			pen->SetDashStyle(style.dStyle);
			int x_Min = (fX > lX) ? lX : fX;
			int y_Min = (fY > lY) ? lY : fY;
			g->DrawRectangle(pen, x_Min, y_Min, abs(lX - fX), abs(lY - fY));

		}
		CShape* Create() { return new CRectangle; }
		void SetData(int a, int b, int c, int d) {
			fX = a;
			fY = b;
			lX = c;
			lY = d;
		}
		void GetData(int& type, int& fx, int& fy, int& lx, int& ly) {
			type = RECTANGLE;
			fx = fX;
			fy = fY;
			lx = lX;
			ly = lY;
		}
		~CRectangle() {}
		LPWSTR GetNameClass() {
			return L"CRectangle";
		}
	};

	class CSquare : public CShape
	{
	public:
		int fX;
		int fY;
		int lX;
		int lY;
	public:
		CSquare() {}
		void Draw(HDC hdc, CStyle style) {
			Graphics* g = new Graphics(hdc);
			g->SetSmoothingMode(SmoothingModeAntiAlias);
			Pen* pen = new Pen(style.color, style.WidthLine);
			pen->SetDashStyle(style.dStyle);
			int edge = (abs(fX - lX) > abs(fY - lY)) ? abs(fY - lY) : abs(fX - lX);
			int x_Min = (fX < lX) ? fX : lX;
			int y_Min = (fY < lY) ? fY : lY;
			if (x_Min == lX) {
				if (y_Min == lY)
					g->DrawRectangle(pen, x_Min + abs(lX - fX) - edge, y_Min + abs(lY - fY) - edge, edge, edge);
				else
					g->DrawRectangle(pen, x_Min + abs(lX - fX) - edge, y_Min, edge, edge);
			}
			else if (y_Min == lY) {
				g->DrawRectangle(pen, x_Min, y_Min + abs(lY - fY) - edge, edge, edge);
			}
			else
				g->DrawRectangle(pen, x_Min, y_Min, edge, edge);
		}
		CShape* Create() { return new CSquare; }
		void SetData(int a, int b, int c, int d) {
			fX = a;
			fY = b;
			lX = c;
			lY = d;
		}

		void GetData(int& type, int& fx, int& fy, int& lx, int& ly) {
			type = SQUARE;
			fx = fX;
			fy = fY;
			lx = lX;
			ly = lY;
		}
		~CSquare() {}
		LPWSTR GetNameClass() {
			return L"CSquare";
		}
	};

	class CElippse : public CShape
	{
	public:
		int fX;
		int fY;
		int lX;
		int lY;
	public:
		CElippse() {}
		void Draw(HDC hdc, CStyle style) {
			Graphics* g = new Graphics(hdc);
			g->SetSmoothingMode(SmoothingModeAntiAlias);
			Pen* pen = new Pen(style.color, style.WidthLine);
			pen->SetDashStyle(style.dStyle);
			int x_Min = (fX > lX) ? lX : fX;
			int y_Min = (fY > lY) ? lY : fY;
			g->DrawEllipse(pen, x_Min, y_Min, abs(lX - fX), abs(lY - fY));
		}
		CShape* Create() { return new CElippse; }
		void SetData(int a, int b, int c, int d) {
			fX = a;
			fY = b;
			lX = c;
			lY = d;
		}
		void GetData(int& type, int& fx, int& fy, int& lx, int& ly) {
			type = ELLIPSE;
			fx = fX;
			fy = fY;
			lx = lX;
			ly = lY;
		}
		~CElippse() {}
		LPWSTR GetNameClass() {
			return L"CElippse";
		}
	};

	class CCircle : public CShape
	{
	public:
		int fX;
		int fY;
		int lX;
		int lY;
	public:
		CCircle() {}

		void Draw(HDC hdc, CStyle style) {
			Graphics* g = new Graphics(hdc);
			g->SetSmoothingMode(SmoothingModeAntiAlias);
			Pen* pen = new Pen(style.color, style.WidthLine);
			pen->SetDashStyle(style.dStyle);
			int edge = (abs(fX - lX) > abs(fY - lY)) ? abs(fX - lX) : abs(fY - lY);
			int x_Min = (fX < lX) ? fX : lX;
			int y_Min = (fY < lY) ? fY : lY;
			if (x_Min == lX) {
				if (y_Min == lY)
					g->DrawEllipse(pen, x_Min + abs(lX - fX) - edge, y_Min + abs(lY - fY) - edge, edge, edge);
				else
					g->DrawEllipse(pen, x_Min + abs(lX - fX) - edge, y_Min, edge, edge);
			}
			else if (y_Min == lY) {
				g->DrawEllipse(pen, x_Min, y_Min + abs(lY - fY) - edge, edge, edge);
			}
			else
				g->DrawEllipse(pen, x_Min, y_Min, edge, edge);
		}
		CShape* Create() { return new CCircle; }

		void SetData(int a, int b, int c, int d) {
			fX = a;
			fY = b;
			lX = c;
			lY = d;
		}
		void GetData(int& type, int& fx, int& fy, int& lx, int& ly) {
			type = CIRCLE;
			fx = fX;
			fy = fY;
			lx = lX;
			ly = lY;
		}
		~CCircle() {}
		LPWSTR GetNameClass() {
			return L"CCircle";
		}
	};

}