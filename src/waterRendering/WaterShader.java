package waterRendering;

import shaders.ShaderProgram;
import shaders.UniformFloat;
import shaders.UniformMatrix;
import shaders.UniformSampler;
import shaders.UniformVec2;
import shaders.UniformVec3;
import utils.MyFile;

public class WaterShader extends ShaderProgram {

	protected static final int REFLECT_TEX_UNIT = 0;
	protected static final int REFRACT_TEX_UNIT = 1;
	protected static final int DEPTH_TEX_UNIT = 2;

	private static final MyFile VERTEX_SHADER = new MyFile("waterRendering", "waterVertex.glsl");
	private static final MyFile FRAGMENT_SHADER = new MyFile("waterRendering", "waterFragment.glsl");

	protected UniformMatrix projectionViewMatrix = new UniformMatrix("projectionViewMatrix");
	protected UniformFloat height = new UniformFloat("height");
	protected UniformVec3 cameraPos = new UniformVec3("cameraPos");
	protected UniformVec2 nearFarPlanes = new UniformVec2("nearFarPlanes");
	protected UniformFloat waveTime = new UniformFloat("waveTime");

	protected UniformSampler reflectionTexture = new UniformSampler("reflectionTexture");
	protected UniformSampler refractionTexture = new UniformSampler("refractionTexture");
	protected UniformSampler depthTexture = new UniformSampler("depthTexture");

	public WaterShader() {
		super(VERTEX_SHADER, FRAGMENT_SHADER);
		super.storeAllUniformLocations(projectionViewMatrix, height, reflectionTexture, refractionTexture, depthTexture,
				cameraPos, nearFarPlanes, waveTime);
		linkTextureUnits();
	}

	private void linkTextureUnits() {
		super.start();
		reflectionTexture.loadTexUnit(REFLECT_TEX_UNIT);
		refractionTexture.loadTexUnit(REFRACT_TEX_UNIT);
		depthTexture.loadTexUnit(DEPTH_TEX_UNIT);
		super.stop();
	}

}
