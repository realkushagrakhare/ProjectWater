package fbos;

public abstract class Attachment {

	private int bufferId;
	private boolean isDepthAttach = false;

	public int getBufferId() {
		return bufferId;
	}

	/**
	 * Creates the attachment by initializing the necessary storage, and then
	 * attaches it to the FBO. This method also needs to set the bufferId after
	 * the storage has been initialized.
	 * 
	 * @param attachment
	 *            - The type of attachment, e.g. GL_COLOR_ATTACHMENT0, or
	 *            GL_DEPTH_ATTACHMENT, etc.
	 * @param width
	 *            - The width of the FBO in pixels
	 * @param height
	 *            - The height of the FBO in pixels
	 * @param samples
	 *            - The number of samples that this FBO uses (for
	 *            multisampling). This is 0 if multisampling is not used.
	 */
	public abstract void init(int attachment, int width, int height, int samples);

	public abstract void delete();

	protected void setBufferId(int id) {
		this.bufferId = id;
	}
	
	protected void setAsDepthAttachment(){
		this.isDepthAttach = true;
	}
	
	protected boolean isDepthAttachment(){
		return isDepthAttach;
	}

}
