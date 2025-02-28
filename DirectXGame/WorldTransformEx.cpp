#include <3d/WorldTransform.h>
#include<math/Matrix4x4.h>

namespace KamataEngine {
	void WorldTransform::UpdateMatrix()
	{
		matWorld_ = MakeAffineMatrix(scale_, rotation_, translation_);
		TransferMatrix();
	}
}