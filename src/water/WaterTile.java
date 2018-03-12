package water;

import openglObjects.Vao;

public class WaterTile {
	
	private final Vao vao;
	private final int vertexCount;
	private final float height;
	
	protected WaterTile(Vao vao, int vertexCount, float height){
		this.vao = vao;
		this.height = height;
		this.vertexCount = vertexCount;
	}

	public Vao getVao() {
		return vao;
	}
	
	public float getHeight(){
		return height;
	}

	public int getVertexCount() {
		return vertexCount;
	}
	
	public void delete(){
		vao.delete(true);
	}

}
