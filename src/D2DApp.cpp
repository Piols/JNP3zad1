#define _USE_MATH_DEFINES

#include <d2d1_3.h>
#include "D2DApp.h"
#include "points.h"
#include <cmath>

#include <iostream>

namespace {
	const INT NPOINTS = 51;
	const FLOAT BRUSHSTROKE = 1.0f;

	// Deklaracje u¿ycia pomocniczych funkcji
	using D2D1::RenderTargetProperties;
	using D2D1::HwndRenderTargetProperties;
	using D2D1::SizeU;
	using D2D1::Point2F;



	// Interfejsy potrzebne do zainicjowania Direct2D
	ID2D1Factory7* d2d_factory = nullptr;
	ID2D1HwndRenderTarget* d2d_render_target = nullptr;

	ID2D1SolidColorBrush* brush = nullptr;
	D2D1_COLOR_F const clear_color =
		{ .r = 0.0f, .g = 0.0f, .b = 0.75f, .a = 1.0f };
	D2D1_COLOR_F const brush_color_1 =
		{ .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f };

	point points[NPOINTS][NPOINTS];
}

void InitDirect2D(HWND hwnd) {
	// Utworzenie fabryki Direct2D
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2d_factory);
	if (d2d_factory == NULL) {
		exit(2);
	}
	RecreateRenderTarget(hwnd);
}

void RecreateRenderTarget(HWND hwnd) {
	RECT rc;
	GetClientRect(hwnd, &rc);
	// Utworzenie celu renderowania w oknie Windows
	d2d_factory->CreateHwndRenderTarget(
		RenderTargetProperties(),
		HwndRenderTargetProperties(hwnd,
			SizeU(static_cast<UINT32>(rc.right) -
				static_cast<UINT32>(rc.left),
				static_cast<UINT32>(rc.bottom) -
				static_cast<UINT32>(rc.top))),
		&d2d_render_target);

	if (d2d_render_target == NULL) {
		exit(3);
	}

	d2d_render_target->CreateSolidColorBrush(brush_color_1, &brush);
}

void DestroyRenderTarget() {
	if (d2d_render_target) {
		d2d_render_target->Release();
		d2d_render_target = nullptr;
	}
}

void DestroyDirect2D() {
	// Bezpieczne zwolnienie zasobów
	if (d2d_render_target) d2d_render_target->Release();
	if (d2d_factory) d2d_factory->Release();
}

void OnPaint(HWND hwnd, FLOAT rotation) {
	if (!d2d_render_target) RecreateRenderTarget(hwnd);

	d2d_render_target->BeginDraw();
	d2d_render_target->Clear(clear_color);

	RECT rc;
	GetClientRect(hwnd, &rc);

	FLOAT half_height = (rc.bottom - rc.top) / 2.0f;
	FLOAT half_width = (rc.right - rc.left) / 2.0f;
	FLOAT scale = 300.0f;

	FLOAT delta = 2.0f / NPOINTS;
	for (int i = 0; i < NPOINTS; i++) {
		for (int j = 0; j < NPOINTS; j++) {
			points[i][j].x = j * delta - 1;
			points[i][j].y = i * delta - 1;
			points[i][j].z = std::cos(10 * std::sqrt(points[i][j].x * points[i][j].x + points[i][j].y * points[i][j].y)) / 4;
			rotate(points[i][j].x, points[i][j].y, M_PI_4);
			rotate(points[i][j].y, points[i][j].z, rotation);
			points[i][j].x = half_width + points[i][j].x * scale;
			points[i][j].y = half_height - points[i][j].y * scale;
		}
	}

	for (int i = 0; i < NPOINTS; i++) {
		for (int j = 0; j < NPOINTS; j++) {
			if (i != NPOINTS - 1) {
				d2d_render_target->DrawLine(
					Point2F(points[i][j].x, points[i][j].y),
					Point2F(points[i + 1][j].x, points[i + 1][j].y),
					brush, BRUSHSTROKE);
			}
			if (j != NPOINTS - 1) {
				d2d_render_target->DrawLine(
					Point2F(points[i][j].x, points[i][j].y),
					Point2F(points[i][j + 1].x, points[i][j + 1].y),
					brush, BRUSHSTROKE);
			}
		}
	}


	if (d2d_render_target->EndDraw() == D2DERR_RECREATE_TARGET) {
		DestroyRenderTarget();
		OnPaint(hwnd, rotation);
	}
}