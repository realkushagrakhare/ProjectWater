package rendering;

import org.lwjgl.input.Keyboard;
import org.lwjgl.opengl.GL11;
import org.lwjgl.opengl.GL14;
import org.lwjgl.opengl.GL30;
import org.lwjgl.opengl.GL32;
import org.lwjgl.util.vector.Vector4f;

import display.Window;
import fbos.Attachment;
import fbos.Fbo;
import fbos.RenderBufferAttachment;
import fbos.TextureAttachment;
import terrains.Terrain;
import utils.OpenGlUtils;
import water.WaterTile;
import waterRendering.WaterRenderer;

public class RenderEngine {
	
	private static final float OFFSET = 2f;

	private final Window window;
	private final WaterRenderer waterRenderer;
	private final Fbo reflectionFbo;
	private final Fbo refractionFbo;

	public RenderEngine(int fps, int displayWidth, int displayHeight) {
		this.window = Window.newWindow(displayWidth, displayHeight, fps).antialias(true).create();
		this.waterRenderer = new WaterRenderer();
		this.refractionFbo = createWaterFbo(displayWidth / 2, displayHeight / 2, true);
		this.reflectionFbo = createWaterFbo(displayWidth, displayHeight, false);
	}

	public void render(Terrain terrain, WaterTile water, ICamera camera, Light light) {
		GL11.glEnable(GL30.GL_CLIP_DISTANCE0);
		doReflectionPass(terrain, camera, light, water.getHeight());
		doRefractionPass(terrain, camera, light, water.getHeight());
		GL11.glDisable(GL30.GL_CLIP_DISTANCE0);
		doMainRenderPass(terrain, water, camera, light);
	}

	public Window getWindow() {
		return window;
	}

	public void close() {
		reflectionFbo.delete();
		refractionFbo.delete();
		waterRenderer.cleanUp();
		window.destroy();
	}

	private void prepare() {
		GL11.glClear(GL11.GL_COLOR_BUFFER_BIT | GL11.GL_DEPTH_BUFFER_BIT);
		GL32.glProvokingVertex(GL32.GL_FIRST_VERTEX_CONVENTION);
		OpenGlUtils.cullBackFaces(true);
		OpenGlUtils.enableDepthTesting(true);
		OpenGlUtils.antialias(true);
	}

	private void doReflectionPass(Terrain terrain, ICamera camera, Light light, float waterHeight) {
		reflectionFbo.bindForRender(0);
		camera.reflect();
		prepare();
		terrain.render(camera, light, new Vector4f(0, 1, 0, -waterHeight));
		camera.reflect();
		reflectionFbo.unbindAfterRender();
	}

	private void doRefractionPass(Terrain terrain, ICamera camera, Light light, float waterHeight) {
		refractionFbo.bindForRender(0);
		prepare();
		terrain.render(camera, light, new Vector4f(0, -1, 0, waterHeight + OFFSET));
		refractionFbo.unbindAfterRender();
	}

	private void doMainRenderPass(Terrain terrain, WaterTile water, ICamera camera, Light light) {
		prepare();
		terrain.render(camera, light, new Vector4f(0, 0, 0, 0));
		OpenGlUtils.goWireframe(Keyboard.isKeyDown(Keyboard.KEY_G));
		waterRenderer.render(water, camera, light, reflectionFbo.getColourBuffer(0), refractionFbo.getColourBuffer(0),
				refractionFbo.getDepthBuffer());
		OpenGlUtils.goWireframe(false);
		window.update();
	}

	private static Fbo createWaterFbo(int width, int height, boolean useTextureForDepth) {
		Attachment colourAttach = new TextureAttachment(GL11.GL_RGBA8);
		Attachment depthAttach;
		if (useTextureForDepth) {
			depthAttach = new TextureAttachment(GL14.GL_DEPTH_COMPONENT24);
		} else {
			depthAttach = new RenderBufferAttachment(GL14.GL_DEPTH_COMPONENT24);
		}
		return Fbo.newFbo(width, height).addColourAttachment(0, colourAttach).addDepthAttachment(depthAttach).init();
	}

}
