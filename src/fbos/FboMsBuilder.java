package fbos;

public class FboMsBuilder {
	
	private final FboBuilder fboBuilder;
	
	protected FboMsBuilder(FboBuilder fboBuilder){
		this.fboBuilder = fboBuilder;
	}
	
	public FboMsBuilder addColourAttachment(int index, RenderBufferAttachment attachment){
		fboBuilder.addColourAttachment(index, attachment);
		return this;
	}
	
	public FboMsBuilder addDepthAttachment(RenderBufferAttachment attachment){
		fboBuilder.addDepthAttachment(attachment);
		return this;
	}
	
	public Fbo init(){
		return fboBuilder.init();
	}

}
