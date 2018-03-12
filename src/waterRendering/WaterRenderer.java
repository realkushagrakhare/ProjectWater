package waterRendering;

import org.lwjgl.opengl.GL11;
import org.lwjgl.opengl.GL13;

import rendering.ICamera;
import rendering.Light;
import utils.OpenGlUtils;
import water.WaterTile;

public class WaterRenderer {

	private final WaterShader shader;

	public WaterRenderer() {
		this.shader = new WaterShader();
	}

	public void render(WaterTile water, ICamera camera, Light light, int reflectionTexture, int refractionTexture, int depthTexture) {
		prepare(water, camera, light);
		bindTextures(reflectionTexture, refractionTexture, depthTexture);
		GL11.glDrawArrays(GL11.GL_TRIANGLES, 0, water.getVertexCount());
		finish(water);
	}

	public void cleanUp() {
		shader.cleanUp();
	}

	private void prepare(WaterTile water, ICamera camera, Light light) {
		water.getVao().bind();
		OpenGlUtils.enableAlphaBlending();
		prepareShader(water, camera, light);
	}
	
	private void bindTextures(int reflectionTexture, int refractionTexture, int depthTexture){
		bindTextureToUnit(reflectionTexture, WaterShader.REFLECT_TEX_UNIT);
		bindTextureToUnit(refractionTexture, WaterShader.REFRACT_TEX_UNIT);
		bindTextureToUnit(depthTexture, WaterShader.DEPTH_TEX_UNIT);
	}
	
	private void bindTextureToUnit(int textureId, int textureUnit){
		GL13.glActiveTexture(GL13.GL_TEXTURE0 + textureUnit);
		GL11.glBindTexture(GL11.GL_TEXTURE_2D, textureId);
	}

	private void finish(WaterTile water) {
		water.getVao().unbind();
		shader.stop();
		OpenGlUtils.disableBlending();
	}
	
	private void prepareShader(WaterTile water, ICamera camera, Light light){
		shader.start();
		loadCameraVariables(camera);
		shader.height.loadFloat(water.getHeight());
	}

	private void loadCameraVariables(ICamera camera){
		shader.projectionViewMatrix.loadMatrix(camera.getProjectionViewMatrix());
		shader.cameraPos.loadVec3(camera.getPosition());
		shader.nearFarPlanes.loadVec2(camera.getNearPlane(), camera.getFarPlane());
	}


}
