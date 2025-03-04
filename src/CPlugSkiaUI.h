#pragma once
#include <out\Release\gen\skia.h>
class SkKnobControl {
private:
    float mValue;         // 0.0 to 1.0
    float mMinValue;
    float mMaxValue;
    SkRect mBounds;
    bool mIsDragging;
    SkPoint mLastPoint;

public:
    SkKnobControl(float x, float y, float width, float height,
        float minValue = 0.0f, float maxValue = 1.0f, float initialValue = 0.0f)
        : mBounds(SkRect::MakeXYWH(x, y, width, height))
        , mMinValue(minValue)
        , mMaxValue(maxValue)
        , mValue(normalizeValue(initialValue))
        , mIsDragging(false)
    {
    }

    float getValue() const {
        return denormalizeValue(mValue);
    }

    void setValue(float value) {
        mValue = normalizeValue(value);
    }

    bool hitTest(float x, float y) const {
        SkPoint center = SkPoint::Make(mBounds.centerX(), mBounds.centerY());
        float radius = std::min(mBounds.width(), mBounds.height()) / 2.0f;
        float distSq = (x - center.x()) * (x - center.x()) + (y - center.y()) * (y - center.y());
        return distSq <= radius * radius;
    }

    void onMouseDown(float x, float y) {
        if (hitTest(x, y)) {
            mIsDragging = true;
            mLastPoint = SkPoint::Make(x, y);
        }
    }

    void onMouseDrag(float x, float y) {
        if (mIsDragging) {
            // Vertical drag to change value (like iPlug2)
            float delta = (mLastPoint.y() - y) / 100.0f;
            mValue = std::clamp(mValue + delta, 0.0f, 1.0f);
            mLastPoint = SkPoint::Make(x, y);
        }
    }

    void onMouseUp(float x, float y) {
        mIsDragging = false;
    }

    void draw(SkCanvas* canvas) {
        SkPoint center = SkPoint::Make(mBounds.centerX(), mBounds.centerY());
        float radius = std::min(mBounds.width(), mBounds.height()) / 2.0f - 2.0f;

        // Draw background circle
        SkPaint bgPaint;
        bgPaint.setAntiAlias(true);
        bgPaint.setColor(SkColorSetRGB(60, 60, 60));
        canvas->drawCircle(center.x(), center.y(), radius, bgPaint);

        // Draw knob indicator
        SkPaint knobPaint;
        knobPaint.setAntiAlias(true);
        knobPaint.setColor(SkColorSetRGB(200, 200, 200));
        knobPaint.setStyle(SkPaint::kStroke_Style);
        knobPaint.setStrokeWidth(2.0f);

        float angle = (mValue * 270.0f - 135.0f) * SK_ScalarPI / 180.0f;
        float indicatorLength = radius * 0.8f;

        SkPoint indicatorPoint = SkPoint::Make(
            center.x() + std::cos(angle) * indicatorLength,
            center.y() + std::sin(angle) * indicatorLength
        );

        canvas->drawLine(center.x(), center.y(), indicatorPoint.x(), indicatorPoint.y(), knobPaint);

        // Draw outer ring
        SkPaint outlinePaint;
        outlinePaint.setAntiAlias(true);
        outlinePaint.setColor(SkColorSetRGB(100, 100, 100));
        outlinePaint.setStyle(SkPaint::kStroke_Style);
        outlinePaint.setStrokeWidth(1.0f);
        canvas->drawCircle(center.x(), center.y(), radius, outlinePaint);
    }

private:
    float normalizeValue(float value) const {
        return (value - mMinValue) / (mMaxValue - mMinValue);
    }

    float denormalizeValue(float normalized) const {
        return mMinValue + normalized * (mMaxValue - mMinValue);
    }
};
class CPlugSkiaUI
{
private:
public:
	void RenderUI(uint32_t* pixelBuffer, int width, int height) {
		SkImageInfo info = SkImageInfo::Make(width, height, kBGRA_8888_SkColorType, kPremul_SkAlphaType);
		sk_sp<SkSurface> surface{};
		surface.get();
		if (!surface) {
			printf("Failed to create Skia surface!\n");
			return;
		}
		SkCanvas* canvas = surface->getCanvas();
		canvas->clear(SK_ColorTRANSPARENT);
		SkPaint paint;
		paint.setColor(SK_ColorRED);
		paint.setAntiAlias(true);
		canvas->drawCircle(width / 2, height / 2, height / 4, paint);
		int stride = width;
		surface->readPixels(info, pixelBuffer, stride * sizeof(uint32_t), 0, 0);
	}
};

