require './page_rank.rb'

page1 = Page.new('page 1')
page2 = Page.new('page 2')
page3 = Page.new('page 3')
page1.add_link(page2, page3)
page1.add_link(page3)
page2.add_link(page3)


describe 'Page' do

  it 'has a name' do
    page1.name.should eq("page 1")
  end

  it 'has links' do
    page1.links.should_not be_nil
  end

  it 'links to other pages' do
    page1.links.should     include page2
    page2.links.should_not include page1
  end

  it 'is linked by other pages' do
    page2.linked_by.should include page1
  end
end


describe 'Ranker' do

  ranker = Ranker.new(page1, page2, page3)

  it 'contains several pages' do
    ranker.pages.size.should_not eq(0)
  end

  it 'computes rank' do
    ranker.rank.should_not be_nil
  end

  it 'is not converging in the beginning' do
    ranker.rank_pages(0.85)
    ranker.converging?.should_not be_true
  end

  it 'ends after several ranks' do
    50.times { ranker.rank_pages(0.85) }
    ranker.converging?.should be_true
  end
end

