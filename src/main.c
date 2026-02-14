#include "raylib.h"

// Định nghĩa macro tính toán
#define MIN(a, b) ((a) < (b) ? (a) : (b))

int main(void)
{
	// Cấu hình cửa sổ
	// FLAG_WINDOW_RESIZABLE: Cho phép kéo giãn cửa sổ
	// FLAG_VSYNC_HINT: Giới hạn FPS theo tần số quét màn hình (mượt hơn)
	SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
	InitWindow(1280, 720, "Raylib Pure C - Auto Scaling");

	// Độ phân giải nội bộ cố định (Virtual Resolution)
	const int virtualResWidth = 1920;
	const int virtualResHeight = 1080;

	// Tạo Render Texture - Đây là cái "khung tranh ảo"
	RenderTexture2D target = LoadRenderTexture(virtualResWidth, virtualResHeight);

	// Đặt bộ lọc Bilinear để khi phóng to hình ảnh không bị quá vỡ (pixelated)
	SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

	// Bật chế độ Fullscreen ngay khi khởi động
	if (!IsWindowFullscreen())
	{
		// ToggleFullscreen();
	}

	// SetTargetFPS(60); // Có thể dùng nếu không muốn dùng VSYNC

	while (!WindowShouldClose())
	{
		// Tính toán Tỷ lệ (Scale) dựa trên kích thước cửa sổ hiện tại
		float windowWidth = (float)GetScreenWidth();
		float windowHeight = (float)GetScreenHeight();

		// Tìm tỷ lệ nhỏ nhất để game luôn nằm trọn trong màn hình
		float scale = MIN(windowWidth / (float)virtualResWidth, windowHeight / (float)virtualResHeight);

		// VẼ NỘI DUNG GAME (Vẽ vào Texture)
		BeginTextureMode(target);
		ClearBackground(RAYWHITE); // Xóa nền của canvas ảo

		// Vẽ mọi thứ theo tọa độ 1920x1080
		DrawText("Day la do phan giai 1920x1080", 100, 100, 40, DARKGRAY);
		DrawRectangle(200, 200, 400, 400, RED);
		DrawCircle(virtualResWidth / 2, virtualResHeight / 2, 100, BLUE);
		EndTextureMode();

		// VẼ CANVAS LÊN MÀN HÌNH (Scale và căn giữa)
		BeginDrawing();
		ClearBackground(BLACK); // Nền đen cho các khoảng trống (Letterbox)

		// Vẽ Texture lên màn hình thực tế
		// Lưu ý: sourceRec có height âm (-target.texture.height) vì texture trong OpenGL bị ngược y
		Rectangle sourceRec = {0.0f, 0.0f, (float)target.texture.width, (float)-target.texture.height};

		// Tính toán vị trí để canvas ảo luôn nằm giữa màn hình thực
		Rectangle destRec = {
			(windowWidth - ((float)virtualResWidth * scale)) * 0.5f,
			(windowHeight - ((float)virtualResHeight * scale)) * 0.5f,
			(float)virtualResWidth * scale,
			(float)virtualResHeight * scale};

		Vector2 origin = {0.0f, 0.0f};
		DrawTexturePro(target.texture, sourceRec, destRec, origin, 0.0f, WHITE);

		DrawFPS(10, 10);
		EndDrawing();
	}

	// Giải phóng bộ nhớ
	UnloadRenderTexture(target);
	CloseWindow();

	return 0;
}