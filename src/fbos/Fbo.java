package fbos;

import java.util.Map;

import org.lwjgl.opengl.Display;
import org.lwjgl.opengl.GL11;
import org.lwjgl.opengl.GL30;

public class Fbo {

	private final int fboId;
	private final int width;
	private final int height;

	private final Map<Integer, Attachment> colourAttachments;
	private final Attachment depthAttachment;

	protected Fbo(int fboId, int width, int height, Map<Integer, Attachment> attachments, Attachment depthAttachment) {
		this.fboId = fboId;
		this.width = width;
		this.height = height;
		this.colourAttachments = attachments;
		this.depthAttachment = depthAttachment;
	}

	public void blitToScreen(int colourIndex) {
		GL30.glBindFramebuffer(GL30.GL_DRAW_FRAMEBUFFER, 0);
		GL11.glDrawBuffer(GL11.GL_BACK);
		GL30.glBindFramebuffer(GL30.GL_READ_FRAMEBUFFER, fboId);
		GL11.glReadBuffer(GL30.GL_COLOR_ATTACHMENT0 + colourIndex);
		GL30.glBlitFramebuffer(0, 0, width, height, 0, 0, Display.getWidth(), Display.getHeight(),
				GL11.GL_COLOR_BUFFER_BIT, GL11.GL_NEAREST);
		GL30.glBindFramebuffer(GL30.GL_FRAMEBUFFER, 0);
	}

	public void blitToFbo(int srcColourIndex, Fbo target, int targetColourIndex) {
		GL30.glBindFramebuffer(GL30.GL_DRAW_FRAMEBUFFER, target.fboId);
		GL11.glDrawBuffer(GL30.GL_COLOR_ATTACHMENT0 + targetColourIndex);

		GL30.glBindFramebuffer(GL30.GL_READ_FRAMEBUFFER, fboId);
		GL11.glReadBuffer(GL30.GL_COLOR_ATTACHMENT0 + srcColourIndex);

		int bufferBit = depthAttachment != null && target.depthAttachment != null
				? GL11.GL_COLOR_BUFFER_BIT | GL11.GL_DEPTH_BUFFER_BIT : GL11.GL_COLOR_BUFFER_BIT;
		GL30.glBlitFramebuffer(0, 0, width, height, 0, 0, target.width, target.height, bufferBit, GL11.GL_NEAREST);
		GL30.glBindFramebuffer(GL30.GL_FRAMEBUFFER, 0);
	}

	public int getColourBuffer(int colourIndex) {
		return colourAttachments.get(colourIndex).getBufferId();
	}

	public int getDepthBuffer() {
		return depthAttachment.getBufferId();
	}
	
	public boolean isComplete(){
		GL30.glBindFramebuffer(GL30.GL_FRAMEBUFFER, fboId);
		boolean complete = GL30.glCheckFramebufferStatus(GL30.GL_FRAMEBUFFER) == GL30.GL_FRAMEBUFFER_COMPLETE;
		GL30.glBindFramebuffer(GL30.GL_FRAMEBUFFER, 0);
		return complete;
	}

	public void bindForRender(int colourIndex) {
		//should add support for binding multiple colour attachments
		GL11.glDrawBuffer(GL30.GL_COLOR_ATTACHMENT0 + colourIndex);
		GL30.glBindFramebuffer(GL30.GL_DRAW_FRAMEBUFFER, fboId);
		GL11.glViewport(0, 0, width, height);
	}

	public void unbindAfterRender() {
		GL30.glBindFramebuffer(GL30.GL_DRAW_FRAMEBUFFER, 0);
		GL11.glViewport(0, 0, Display.getWidth(), Display.getHeight());
	}

	public void delete() {
		for (Attachment attachment : colourAttachments.values()) {
			attachment.delete();
		}
		if (depthAttachment != null) {
			depthAttachment.delete();
		}
	}

	public static FboBuilder newFbo(int width, int height) {
		return new FboBuilder(width, height, 0);
	}

	public static FboMsBuilder newMultisampledFbo(int width, int height, int samples) {
		return new FboMsBuilder(new FboBuilder(width, height, samples));
	}

}
