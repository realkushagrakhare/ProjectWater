package water;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

import org.lwjgl.util.vector.Vector2f;

import openglObjects.Vao;
import vertexDataStoring.DataStoring;
import vertexDataStoring.VaoLoader;

public class WaterGenerator {

	private static final int VERTICES_PER_SQUARE = 3 * 2;// 2 triangles
	private static final int VERTEX_SIZE_BYTES = 8 + 4;// x,z position +
														// indicator

	public static WaterTile generate(int gridCount, float height) {
		int totalVertexCount = gridCount * gridCount * VERTICES_PER_SQUARE;
		byte[] waterMeshData = createMeshData(gridCount, totalVertexCount);
		Vao vao = VaoLoader.createWaterVao(waterMeshData);
		return new WaterTile(vao, totalVertexCount, height);
	}

	private static byte[] createMeshData(int gridCount, int totalVertexCount) {
		int byteSize = VERTEX_SIZE_BYTES * totalVertexCount;
		ByteBuffer buffer = ByteBuffer.allocate(byteSize).order(ByteOrder.nativeOrder());
		for (int row = 0; row < gridCount; row++) {
			for (int col = 0; col < gridCount; col++) {
				storeGridSquare(col, row, buffer);
			}
		}
		return buffer.array();
	}

	private static void storeGridSquare(int col, int row, ByteBuffer buffer) {
		Vector2f[] cornerPos = calculateCornerPositions(col, row);
		storeTriangle(cornerPos, buffer, true);
		storeTriangle(cornerPos, buffer, false);
	}

	private static void storeTriangle(Vector2f[] cornerPos, ByteBuffer buffer, boolean left) {
		int index0 = left ? 0 : 2;
		byte[] indicators = {0,0,0,0};
		DataStoring.packVertexData(cornerPos[index0], indicators, buffer);
		int index1 = 1;
		DataStoring.packVertexData(cornerPos[index1], indicators, buffer);
		int index2 = left ? 2 : 3;
		DataStoring.packVertexData(cornerPos[index2], indicators, buffer);
	}

	private static Vector2f[] calculateCornerPositions(int col, int row) {
		Vector2f[] vertices = new Vector2f[4];
		vertices[0] = new Vector2f(col, row);
		vertices[1] = new Vector2f(col, row + 1);
		vertices[2] = new Vector2f(col + 1, row);
		vertices[3] = new Vector2f(col + 1, row + 1);
		return vertices;
	}


}
